from django.shortcuts import render

from workers.game_creator import GameCreator

def Home(request):

    gc = GameCreator()

    #print('Current Info: ')
    #for x in request.META:
    #    gc.print_val('{}: {}'.format(x, request.META.get(x)))

    context = {'first_val':'worked', 'second_val':'worked too'}
    return render(request, 'home.html', context)

def GameCustiomize(request):

    user_ip = request.META.get('REMOTE_ADDR')
    user_name = request.META.get('USERNAME')
    print('User IP:{}\nUser Name: {}'.format(user_ip, user_name))
    print('URL: {}\n\n'.format(request.build_absolute_uri()))
    context = {'first_val':'worked', 'second_val':'worked too'}
    return render(request, 'customize.html', context)

def GameRoom(request):
    context = {'first_val':'worked', 'second_val':'worked too'}
    return render(request, 'play.html', context)