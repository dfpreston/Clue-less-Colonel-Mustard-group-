from django.shortcuts import render

from workers.game_manager import GameManager
from workers.player_manager import PlayerManager

# Will convert theses to classes once done
def Home(request):
    """
    :param request:
    :return:
    """
    # Server to Client data pushes
    if request.method == 'GET':
        print('Get Request!')
    # Client to Server data updates
    elif request.method == 'POST':
        print('Post Request!')

    user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('USERNAME')

    # GAME CREATION TESTING
    gm = GameManager()

    gm.delete_completed_games()  # Cleaning for testing
    gm.initialize_new_game(client_ip=user_ip, client_name=user_name)
    gm.start_game(client_ip=user_ip, client_name=user_name)

    # PLAYER STATUS TESTING
    pm = PlayerManager(client_ip=user_ip, client_name=user_name)

    context = {'player_location':pm.get_player_location(),
               'others_locations':pm.get_other_player_locations(),
               'player_hand':pm.get_hand(),
               'available_cards':pm.get_unused_cards()}

    return render(request, 'home.html', context)

def GameCustiomize(request):
    """
    Can potentially be call the waiting room

    :param request:
    :return:
    """

    user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('USERNAME')
    print('User IP:{}\nUser Name: {}'.format(user_ip, user_name))
    print('URL: {}\n\n'.format(request.build_absolute_uri()))
    context = {'first_val':'worked', 'second_val':'worked too'}
    return render(request, 'customize.html', context)

def GameRoom(request):
    context = {'first_val':'worked', 'second_val':'worked too'}
    return render(request, 'play.html', context)