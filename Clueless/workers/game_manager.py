import random
import math

from Clueless.models import Games, Cards, Players, Locations


class GameManager:
    """
    This class will be used to manage/construct each overall game
    Dirty coding now to get the overall struture of everything right
    Will clean as we go
    """

    def __init__(self, game_id=None):
        """
        Dirty way to make sure everything works
        Will change in future
        """
        self.game_id = game_id
        self.rooms = {'Room':['room1', 'room2', 'room3', 'room4',
                      'room5', 'room6', 'room7', 'room8', 'room9'],
                      'Hallway':['hallway1', 'hallway2', 'hallway3', 'hallway4',
                                 'hallway5', 'hallway6', 'hallway7', 'hallway8',
                                 'hallway9', 'hallway10', 'hallway11', 'hallway12']}
        self.cards = {'Weapons':['weapon1', 'weapon2', 'weapon3', 'weapon4'],
                      'Suspects':['suspect1', 'suspect2', 'suspect3','suspect4'],
                      'Rooms':['room1', 'room2', 'room3', 'room4']}
        self.game_status = ['PENDING', 'IN_PROGRESS', 'COMPLETED']

    # Game initialization
    def create_new_game(self):
        Games.objects.create(status=self.game_status[0])
        self.game_id = Games.objects.all().latest('created_at')

    def create_deck(self):
        for card_type in self.cards:
            for card_name in self.cards[card_type]:
                Cards.objects.create(game=self.game_id, card_type=card_type, name=card_name)

    def create_locations(self):
        for t in self.rooms:
            for room in self.rooms[t]:
                Locations.objects.create(game=self.game_id, name=room, type=t)

    def add_player(self, client_ip, client_name, is_creator):

        #self.player_multi_game_check(client_ip=client_ip, client_name=client_name)

        Players.objects.create(game=self.game_id, name='Blah', game_creator=is_creator,
                               client_ip=client_ip, client_name=client_name)

    def player_multi_game_check(self, client_ip, client_name):
        # Check to see if player is already in a game
        if Players.objects.filter(client_ip=client_ip, client_name=client_name).exists():

            player = Players.objects.filter(client_ip=client_ip, client_name=client_name)[0]

            # Remove game if player is creator
            if player.game_creator:
                Games.objects.filter(id=player.game.id).delete()
            # Remove player from previous game if ont creator
            else:
                player.delete()

    def initialize_new_game(self, client_ip, client_name):
        self.create_new_game()
        self.create_deck()
        self.create_locations()
        self.add_player(client_ip=client_ip, client_name=client_name, is_creator=True)

    # Game Start
    def sort_out_cards(self):

        # Get random solution cards
        for card_type in self.cards:
            solution_card = Cards.objects.filter(game=self.game_id, card_type=card_type).order_by('?')[0]
            solution_card.solution = True
            solution_card.used = True
            solution_card.save()

        # 'Shuffle' deck and hand cards out
        deck = Cards.objects.filter(game=self.game_id, solution=False).order_by('?')

        # Get total number of cards per hand
        player_card_num = math.floor(deck.count() / Players.objects.filter(game=self.game_id).count())

        card_counter = 0
        player_counter = 0
        player_ids = Players.objects.filter(game=self.game_id).values('id')

        # Create hands
        for card in deck:

            # Assign card to player
            card.player = Players.objects.filter(game=self.game_id, id=player_ids[player_counter]['id'])[0]
            card.save()
            card_counter += 1

            # Go to next player hand if current player's hand is finished
            if player_counter < player_ids.count()-1 and card_counter == player_card_num:
                player_counter += 1
                card_counter = 0

    def place_players(self):
        for location in Locations.objects.filter(game=self.game_id, type='Hallway').order_by('?'):

            if Players.objects.filter(game=self.game_id, location__isnull=True).exists():
                player = Players.objects.filter(game=self.game_id, location__isnull=True)[0]
                player.location = location
                player.save()
                print('Player location at game start: {}'.format(player.location.name))
            else:
                break

    def set_player_order(self):
        turn_order = []

        for player in Players.objects.filter(game=self.game_id).order_by('?'):
            turn_order.append(player.id)

        Players.objects.filter(id=turn_order[0]).update(their_turn=True)
        Games.objects.filter(id=self.game_id).update(turn_order=turn_order)

    def start_game(self):

        # Randomly picks solution cards and player hands
        self.sort_out_cards()
        # Set starting location for all players
        self.place_players()
        # Set payer order for game
        self.set_player_order()
        # Mark game in progress
        Games.objects.filter(id=self.game_id).update(status=self.game_status[1])

    # Game Updates
    def update_player_turn(self):
        turn_order = []

        temp = (Games.objects.filter(id=self.game_id)[0].turn_order)\
            .replace('[','').replace(']','').replace(' ','').split(',')

        for player_id in temp:
            turn_order.append(int(player_id))

        # Get current player turn id
        curr_id = Players.objects.filter(game=self.game_id, their_turn=True)[0].id

        # Go to the front of the list, if current player is last
        if len(turn_order)-1 == turn_order.index(curr_id):
            next_id = turn_order[0]
        # Else, go to next player
        else:
            next_id = turn_order[turn_order.index(curr_id)+1]

        # Update player turns
        Players.objects.filter(id=curr_id).update(their_turn=False)
        Players.objects.filter(id=next_id).update(their_turn=True)

        print(turn_order.index(curr_id))

        # Skip players that lost
        if Players.objects.filter(id=next_id)[0].status == 'LOST' and Players.objects.filter(game=self.game_id).count()>1:
            self.update_player_turn()

    def update_game_status(self, client_ip, client_name, player_status):
        if player_status == '':
            return
        if player_status == 'lost':
            Players.objects.filter(game=self.game_id, client_ip=client_ip, client_name=client_name)\
                           .update(status='LOST')

        if player_status == 'won':
            Players.objects.filter(game=self.game_id, client_ip=client_ip, client_name=client_name)\
                           .update(status='WON')

            Games.objects.filter(id=self.game_id).update(status='COMPLETED')



    # Game info returns
    def get_solution_cards(self):
        solution = {}

        for card in Cards.objects.filter(game=self.game_id, solution=True):

            if card.card_type not in solution.keys():
                solution[card.card_type] = [card.name]
            else:
                solution[card.card_type].append(card.name)

        return solution

    def get_game_status(self):
        return (Games.objects.filter(id=self.game_id)[0]).status

    def get_game_winner(self):
        if Players.objects.filter(game=self.game_id, status='WON'):
            return Players.objects.filter(game=self.game_id, status='WON')[0].name
        else:
            return ''

    def get_games_pending(self):
        return Games.objects.filter(status=self.game_status[0]).count()

    def get_games_in_progress(self):
        return Games.objects.filter(status=self.game_status[1]).count()

    def get_player_names(self):
        names = []

        for player in Players.objects.filter(game=self.game_id):
            names.append(player.name)

        return names

    def delete_completed_games(self):
        if Games.objects.filter().exists():
            Games.objects.filter().delete()
