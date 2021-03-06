import json

from django.shortcuts import render

from django.views.decorators.csrf import csrf_exempt
from django.http import HttpResponse
from django.shortcuts import redirect

from workers.game_manager import GameManager
from workers.player_manager import PlayerManager


# Using for testing, will remove
# URL will include game_id in future
from Clueless.models import Games

# Will convert theses to classes once done
def Home(request):
    """
    :param request:
    :return:
    """
    # Server to Client data pushes
    if request.method == 'GET':
        print('Get Request!')

        gm = GameManager()

        context = {'games_pending':gm.get_games_pending(),
                   'games_in_progress':gm.get_games_in_progress(),
                   'game_number': len(gm.get_games_pending())}

        for key in context.keys():
            context[key] = json.dumps(context[key])

    # Client to Server data updates
    elif request.method == 'POST':
        print('Post Request!')

    return render(request, 'home.html', context)

@csrf_exempt
def GameCustiomize(request):
    """
    Can potentially be call the waiting room

    :param request:
    :return:
    """

    # Get Client information
    if 'HTTP_X_REAL_IP' in (request.META).keys():
        user_ip = request.META.get('HTTP_X_REAL_IP')
    else:
        user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('HTTP_USER_AGENT')

    # GET Request
    if request.method == 'GET':

        # User create new game
        if request.GET.get('game') == 'create':
            msg = 'You have created a new game!'

            # Game tracker
            gm = GameManager()
            gm.delete_completed_games()  # Cleaning for testing
            gm.initialize_new_game(client_ip=user_ip, client_name=user_name)

            pm = PlayerManager(client_ip=user_ip, client_name=user_name)
            gm = GameManager(game_id=pm.get_game_id())

        # User joins pending game
        elif request.GET.get('game') == 'join':

            if not request.GET.get('game_id'):
                return redirect('/')

            if not Games.objects.filter(status='PENDING').exists():
                return redirect('/')

            msg = 'You have joined an existing game!'

            # Game tracker
            game = Games.objects.filter(id=int(request.GET.get('game_id')))[0]
            gm = GameManager(game_id=game)

            if len(gm.get_player_names()) > 6:
                return redirect('/')

            gm.add_player(client_ip=user_ip, client_name=user_name, is_creator=False)

        else:
            msg = 'Room has been updated with new info!'
            pm = PlayerManager(client_ip=user_ip, client_name=user_name)
            gm = GameManager(game_id=pm.get_game_id())

    # Post Request
    elif request.method == 'POST':
        msg = 'You are updating player info for game!'
        pm = PlayerManager(client_ip=user_ip, client_name=user_name)
        gm = GameManager(game_id=pm.get_game_id())

        player_token = request.GET.get('token', '')
        player_leave = request.GET.get('leave_game', '')

        if player_leave is not '':
            gm.remove_player(check=player_leave, client_ip=user_ip, client_name=user_name)
            return redirect('/')
        pm.update_player_name(player_token)

    # PLAYER STATUS TESTING
    pm = PlayerManager(client_ip=user_ip, client_name=user_name)
    gm = GameManager(game_id=pm.get_game_id())

    if gm.get_game_status() == 'IN_PROGRESS':
        return redirect('/play')

    context = {'msg':msg,
               'game_id':pm.get_game_id(),
               'player_name':pm.get_player_name(),
               'all_names':gm.get_player_names(),
               'player_hand':pm.get_hand(),
               'available_cards':pm.get_unused_cards(),
               'is_creator': pm.get_is_creator(),
               'game_status': gm.get_game_status()}

    for key in context.keys():
        context[key] = json.dumps(context[key])

    return render(request, 'customize.html', context)

@csrf_exempt
def GameRoom(request):

    # Get Client information
    if 'HTTP_X_REAL_IP' in (request.META).keys():
        user_ip = request.META.get('HTTP_X_REAL_IP')
    else:
        user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('HTTP_USER_AGENT')

    pm = PlayerManager(client_ip=user_ip, client_name=user_name)
    gm = GameManager(game_id=pm.get_game_id())

    if request.method == 'GET':
        """
        Update waiting player
        """
        print('Get Request!')

        if len(gm.get_player_names()) < 0 or '' in gm.get_player_names():
            return redirect('/customize')

        # Start game if new
        if gm.get_game_status() == 'PENDING':
            # Start game and update player with new info
            gm.start_game()
            pm = PlayerManager(client_ip=user_ip, client_name=user_name)

    elif request.method == 'POST':
        """
        Update Player information during turn 
        """
        print('Post Request!')
        print(request.GET.get('used_card', ''))
        print(request.GET.get('new_location', ''))
        print(request.GET.get('player_status', ''))
        print(request.GET.get('player_location', ''))
        print(request.GET.get('suggest_room', ''))
        print(request.GET.get('suggest_weapon', ''))
        print(request.GET.get('suggest_suspect', ''))
        print(request.GET.get('refute_card', ''))

        gm.update_suggested_card(request.GET.get('suggest_room', ''))
        gm.update_suggested_card(request.GET.get('suggest_weapon', ''))
        gm.update_suggested_card(request.GET.get('suggest_suspect', ''))

        if request.GET.get('suggest_suspect_name', '') != '':
            gm.update_suspect_suggest_location(request.GET.get('suggest_suspect_name', ''))
        gm.refute_suggestion(request.GET.get('refute_card', ''))
        gm.update_game_status(user_ip, user_name, request.GET.get('player_status', ''))
        pm.update_player_location(request.GET.get('player_location', ''))

        if request.GET.get('end_turn', '') is not '' or request.GET.get('player_status', '') is not '':
            gm.update_player_turn()

    #gm.update_suspect_suggest_location(request.GET.get('suggest_suspect_name', ''))
    context = {'game_id': pm.get_game_id(),
               'player_name': pm.get_player_name(),
               'player_turn': pm.get_is_player_turn(),
               'curr_player_turn': gm.get_curr_player_turn(),
               'player_moved': pm.get_move_status(),
               'player_moved_suggest': pm.get_move_suggest_status(),
               'player_location': pm.get_player_location(),
               'player_status': pm.get_player_status(),
               'player_suggested': pm.get_suggest_status(),
               'others_locations': pm.get_other_player_locations(),
               'player_hand': pm.get_hand(),
               'available_cards': pm.get_unused_cards(),
               'suggested_cards': gm.get_suggested_cards(),
               'refuted_card': gm.get_refuted_card(),
               'is_creator': pm.get_is_creator(),
               'solution_cards': gm.get_solution_cards(),
               'game_status': gm.get_game_status(),
               'game_winner': gm.get_game_winner(),
               'game_loser': gm.get_game_loser(),
               'weapon_locations': gm.get_weapon_locations(),
               'suspect_locations': gm.get_suspect_locations()}

    for key in context.keys():
        context[key] = json.dumps(context[key])

    return render(request, 'play.html', context)