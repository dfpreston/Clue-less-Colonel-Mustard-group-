from django.test import TestCase
from workers.game_manager import GameManager
from Clueless.models import Games, Cards, Players, Locations

class GameManagerTestCase():
    def setup(self):
        GameManager.initialize_new_game('1.1.1.1','blah')
        GameManager.start_game()

    def test_initial_player_is_creator(self):
        gameid = Games.objects.all()[0]
        player = Players.object.filter(game=gameid,clientip="1.1.1.1",client_name="blah")[0]
        assert(player.gameCreator == "True")
