from django.test import TestCase
from workers.game_manager import GameManager
from workers.player_manager import PlayerManager
from Clueless.models import Games, Cards, Players, Locations

class StartGame():
    def setup(self):
        GameManager.initialize_new_game('1.1.1.1','p1')
        GameManager.start_game()

    def test_initial_player_is_creator(self):
        gameid = Games.objects.all()[0]
        p1 = Players.object.filter(game=gameid,clientip="1.1.1.1",client_name="p1")[0]
        assert(p1.gameCreator == "True")

    def test_players_less_than_3(self):
        pass

    def test_add_seventh_player(self):
        pass

class TestMovePlayer():
    def test_in_room_blocked_hallways(self):
        pass

    def test_player_in_room_no_blocked_hallways(self):
        pass

    def test_player_in_hallway(self):
        pass

class TestMakeSuggestion():


class TestMakeAccusation():
    def test_accusation_correct(self):
        pass

    def test_accusation_not_correct(self):
        pass
