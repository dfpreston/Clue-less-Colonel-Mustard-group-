from django.shortcuts import render

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
                   'games_in_progress':gm.get_games_in_progress}

    # Client to Server data updates
    elif request.method == 'POST':
        print('Post Request!')

    return render(request, 'home.html', context)

def GameCustiomize(request):
    """
    Can potentially be call the waiting room

    :param request:
    :return:
    """

    # GET Request
    if request.method == 'GET':

        # Get Client information
        if 'HTTP_X_REAL_IP' in (request.META).keys():
            user_ip = request.META.get('HTTP_X_REAL_IP')
        else:
            user_ip = request.META.get('REMOTE_ADDR')
        user_name = request.META.get('USERNAME')

        # User create new game
        if request.GET.get('game') == 'create':
            msg = 'You have created a new game!'

            # Game tracker
            gm = GameManager()
            gm.delete_completed_games()  # Cleaning for testing
            gm.initialize_new_game(client_ip=user_ip, client_name=user_name)

        # User joins pending game
        elif request.GET.get('game') == 'join':
            msg = 'You have joined an existing game!'

            # Game tracker
            gm = GameManager(game_id=Games.objects.filter(status='PENDING')[0])
            gm.add_player(client_ip=user_ip, client_name=user_name, is_creator=False)

        else:
            return render(request, 'customize.html')

    # Post Request
    elif request.method == 'POST':
        pass

    # PLAYER STATUS TESTING
    pm = PlayerManager(client_ip=user_ip, client_name=user_name)

    context = {'msg':msg,
               'game_id':pm.get_game_id(),
               'player_hand':pm.get_hand(),
               'available_cards':pm.get_unused_cards(),
               'is_creator': pm.get_is_creator()}

    return render(request, 'customize.html', context)


def GameRoom(request):

    if request.method == 'GET':
        # Get Client information
        if 'HTTP_X_REAL_IP' in (request.META).keys():
            user_ip = request.META.get('HTTP_X_REAL_IP')
        else:
            user_ip = request.META.get('REMOTE_ADDR')
        user_name = request.META.get('USERNAME')

        # TESTING
        # start_game will not run if status is in progress
        # status will update to in progress within start_game
        gm = GameManager(game_id=Games.objects.filter(status='PENDING')[0])
        gm.start_game()

        pm = PlayerManager(client_ip=user_ip, client_name=user_name)

        context = {'game_id': pm.get_game_id(),
                   'player_turn': pm.get_is_player_turn(),
                   'player_location': pm.get_player_location(),
                   'others_locations': pm.get_other_player_locations(),
                   'player_hand': pm.get_hand(),
                   'available_cards': pm.get_unused_cards(),
                   'is_creator':pm.get_is_creator(),
                   'solution_cards':gm.get_solution_cards()}

    elif request.method == 'POST':
        print('Post Request!')

    return render(request, 'play.html', context)