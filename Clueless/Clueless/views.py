from django.shortcuts import render

from workers.game_manager import GameManager

# Will convert theses to classes once done
def Home(request):
    user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('USERNAME')

    gm = GameManager()

    gm.delete_completed_games()  # Cleaning for testing

    gm.initialize_new_game(client_ip=user_ip, client_name=user_name)
    gm.start_game(client_ip=user_ip, client_name=user_name)

    #gm.delete_completed_games()

    context = {'first_val':'worked', 'second_val':'worked too'}

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