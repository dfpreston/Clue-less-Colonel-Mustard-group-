import random
import math

from Clueless.models import Games, Cards, Players, Locations


class GameManager:
    """
    This class will be used to manage/construct each overall game
    Dirty coding now to get the overall struture of everything right
    Will clean as we go
    """

    def __init__(self):
        """
        Dirty way to make sure everything works
        Will change in future
        """
        self.game_id = None
        self.rooms = ['room1', 'room2', 'room3', 'room4',
                      'room5', 'room6', 'room7', 'room8', 'room9',
                      'hallway1', 'hallway2', 'hallway3', 'hallway4', 'hallway5,', 'hallway6',
                      'hallway7', 'hallway8', 'hallway9', 'hallway10', 'hallway11', 'hallway12']
        self.cards = {'Weapons':['weapon1', 'weapon2', 'weapon3'],
                      'Suspects':['suspect1', 'suspect2', 'suspect3'],
                      'Rooms':['room1', 'room2', 'room3']}
        self.game_status = ['PENDING', 'IN_PROGRESS', 'COMPLETED']

    def create_new_game(self):
        Games.objects.create(status=self.game_status[0])
        self.game_id = Games.objects.all().latest('created_at')

    def create_deck(self):
        for card_type in self.cards:
            for card_name in self.cards[card_type]:
                Cards.objects.create(game=self.game_id, card_type=card_type, name=card_name)

    def create_locations(self):
        for room in self.rooms:
            Locations.objects.create(game=self.game_id, name=room)

    def add_player(self, client_ip, client_name, is_creator):
        # Just to test locations
        l = Locations.objects.filter(game=self.game_id)[0]

        Players.objects.create(game=self.game_id, name='Blah', game_creator=is_creator,
                               client_ip=client_ip, client_name=client_name, location=l)

    def initialize_new_game(self, client_ip, client_name):
        self.create_new_game()
        self.create_deck()
        self.create_locations()
        self.add_player(client_ip=client_ip, client_name=client_name, is_creator=True)

    def sort_out_cards(self, client_ip, client_name):

        curr_game = Players.objects.filter(client_ip=client_ip, client_name=client_name)[0].game

        # Get random solution cards
        for card_type in self.cards:
            card_type_num = Cards.objects.filter(game=curr_game, card_type=card_type).count()
            rand_index = random.randint(0, card_type_num - 1)
            rand_card_id = Cards.objects.filter(game=curr_game, card_type=card_type)[rand_index].id
            Cards.objects.filter(id=rand_card_id).update(solution=True, used=True)

        # 'Shuffle' deck and hand cards out
        card_ids = []
        for card in Cards.objects.filter(game=curr_game, solution=False):
            card_ids.append(card.id)

        random.shuffle(card_ids)

        player_card_num = math.floor(len(card_ids) / Players.objects.filter(client_ip=client_ip,
                                                                            client_name=client_name).count())

        last_player_card_num = math.ceil(len(card_ids) / Players.objects.filter(client_ip=client_ip,
                                                                                client_name=client_name).count())

        for player in Players.objects.filter(client_ip=client_ip, client_name=client_name):

            if len(card_ids) <= last_player_card_num:
                for x in range(0, last_player_card_num):
                    Cards.objects.filter(id=card_ids[0]).update(player=player)
                    card_ids.pop(0)
            else:
                for x in range(0, player_card_num):
                    Cards.objects.filter(id=card_ids[0]).update(player=player)
                    card_ids.pop(0)

    def create_room_layout(self, client_ip, client_name):
        pass

    def start_game(self, client_ip, client_name):

        # Randomly picks solution cards and player hands
        self.sort_out_cards(client_ip, client_name)

        #
        self.create_room_layout(client_ip, client_name)

    def delete_completed_games(self):
        Games.objects.filter().delete()
