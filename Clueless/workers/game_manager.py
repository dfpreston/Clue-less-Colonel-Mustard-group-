import random
import math

from Clueless.models import Games, Cards, Players, Locations, Weapons, Suspects


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
                                 'hallway9', 'hallway10', 'hallway11', 'hallway12'],
                      'StartSpace':['mustard_start', 'scarlet_start', 'green_start',
                              'peacock_start', 'white_start', 'plum_start']}
        self.cards = {'Weapons':['weapon1', 'weapon2', 'weapon3', 'weapon4', 'weapon5', 'weapon6'],
                      'Suspects':['suspect1', 'suspect2', 'suspect3','suspect4', 'suspect5', 'suspect6'],
                      'Rooms':['room1', 'room2', 'room3', 'room4', 'room5', 'room6', 'room7', 'room8', 'room9']}
        self.game_status = ['PENDING', 'IN_PROGRESS', 'COMPLETED']
        self.weapons = ['weapon1', 'weapon2', 'weapon3', 'weapon4', 'weapon5', 'weapon6']
        self.suspects = {"Colonel Mustard": "suspect1",
                         "Miss Scarlet": "suspect2",
                         "Mr. Green": "suspect3",
                         "Mrs. Peacock": "suspect4",
                         "Mrs. White": "suspect5",
                         "Professor Plum": "suspect6"}
        self.startlocations = {'Miss Scarlet': 'scarlet_start',
                     'Colonel Mustard': 'mustard_start',
                     'Mrs. White': 'white_start',
                     'Mr. Green': 'green_start',
                     'Mrs. Peacock': 'peacock_start',
                     'Professor Plum': 'plum_start'}

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

    def create_weapons(self):
        rooms = Locations.objects.filter(game=self.game_id, type='Room').order_by('?')
        i = 0
        for weapon in self.weapons:
            Weapons.objects.create(game=self.game_id, name=weapon, location=rooms[i])
            i += 1

    def create_suspects(self):
        for (suspect, location) in zip(self.cards['Suspects'], self.rooms['StartSpace']):
            Suspects.objects.create(game=self.game_id, name=suspect, location=Locations.objects.filter(game=self.game_id, type='StartSpace', name=location)[0])

    def add_player(self, client_ip, client_name, is_creator):

        if Players.objects.filter(client_ip=client_ip, client_name=client_name).exists():
            Players.objects.filter(client_ip=client_ip, client_name=client_name).delete()

        Players.objects.create(game=self.game_id, name='', game_creator=is_creator,
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
        self.create_weapons()
        self.create_suspects()
        self.add_player(client_ip=client_ip, client_name=client_name, is_creator=True)

    # Game Start
    def sort_out_cards(self):

        # Get random solution cards
        for card_type in self.cards:
            solution_card = Cards.objects.filter(game=self.game_id, card_type=card_type).order_by('?')[0]
            solution_card.solution = True
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

    ## @brief: Initializes character token location to associated starting space.
    ## @param: game manager
    def place_players(self):
        for player in Players.objects.filter(game=self.game_id):
            player.location = Locations.objects.filter(game=self.game_id, name=self.startlocations[player.name])[0]
            player.save()

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

        if not Players.objects.filter(game=self.game_id, status='ACTIVE').exists():
            Games.objects.filter(id=self.game_id).update(status='COMPLETED')
            return

        Players.objects.filter(game=self.game_id).update(moved=False, suggested=False)
        Players.objects.filter(game=self.game_id, their_turn=True).update(moved_by_suggest=False)
        Cards.objects.filter(game=self.game_id, solution=False).update(used=False)

        if Players.objects.filter(game=self.game_id, status='LOST', lost=False).exists():
            Players.objects.filter(game=self.game_id, status='LOST', lost=False).update(lost=True)

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
            player = Players.objects.filter(game=self.game_id, client_ip=client_ip, client_name=client_name)[0]
            start_loc = Locations.objects.filter(game=self.game_id, name=self.startlocations[player.name])[0]
            Suspects.objects.filter(game=self.game_id, name=self.suspects[player.name]).update(location=start_loc)
            Players.objects.filter(game=self.game_id, client_ip=client_ip, client_name=client_name)\
                .update(location=start_loc)

        if player_status == 'won':
            Players.objects.filter(game=self.game_id, client_ip=client_ip, client_name=client_name)\
                           .update(status='WON')

            Games.objects.filter(id=self.game_id).update(status='COMPLETED')

    def update_suggested_card(self, card_name):
        if Cards.objects.filter(game=self.game_id, name=card_name).exists():
            Cards.objects.filter(game=self.game_id, name=card_name).update(suggested=True)

        Players.objects.filter(game=self.game_id, their_turn=True).update(suggested=True)

    def update_suspect_suggest_location(self, suspect):
        if Cards.objects.filter(game=self.game_id, card_type='Suspects', suggested=True).exists() and\
           Cards.objects.filter(game=self.game_id, card_type='Rooms', suggested=True).exists():
            room_name = Cards.objects.filter(game=self.game_id, card_type='Rooms', suggested=True)[0].name
            weapon = Cards.objects.filter(game=self.game_id, card_type='Weapons', suggested=True)[0].name
            location = Locations.objects.filter(game=self.game_id, name=room_name)[0]
            Weapons.objects.filter(game=self.game_id, name=weapon).update(location=location)

            Suspects.objects.filter(game=self.game_id, name=self.suspects[suspect]).update(location=location)
            if Players.objects.filter(game=self.game_id, name=suspect).exists() and\
                    not Players.objects.filter(game=self.game_id, name=suspect, location=location).exists():
                Players.objects.filter(game=self.game_id, name=suspect).update(location=location, moved_by_suggest=True)

    def update_weapon_suggest_location(self, weapon_name, location_name):
        new_location = Locations.objects.filter(game=self.game_id, name=location_name)[0]
        Weapons.objects.filter(game=self.game_id, name=weapon_name).update(location=new_location)

    def refute_suggestion(self, card_name):
        if Cards.objects.filter(game=self.game_id, name=card_name).exists():
            #Cards.objects.filter(game=self.game_id, solution=False).update(used=False)
            Cards.objects.filter(game=self.game_id).update(used=False)
            Cards.objects.filter(game=self.game_id, name=card_name).update(used=True)
            Cards.objects.filter(game=self.game_id, suggested=True).update(suggested=False)

    # Game info returns
    def get_solution_cards(self):
        solution = {}

        for card in Cards.objects.filter(game=self.game_id, solution=True):

            if card.card_type not in solution.keys():
                solution[card.card_type] = [card.name]
            else:
                solution[card.card_type].append(card.name)

        return solution

    def get_suggested_cards(self):
        suggests = {}

        for card in Cards.objects.filter(game=self.game_id, suggested=True):
            if card.card_type not in suggests.keys():
                suggests[card.card_type] = [card.name]
            else:
                suggests[card.card_type].append(card.name)
        return suggests

    def get_refuted_card(self):
        if Cards.objects.filter(game=self.game_id, used=True).exists():
            return Cards.objects.filter(game=self.game_id, used=True)[0].name
        else:
            return ''

    def get_game_status(self):
        return (Games.objects.filter(id=self.game_id)[0]).status

    def get_game_winner(self):
        if Players.objects.filter(game=self.game_id, status='WON'):
            return Players.objects.filter(game=self.game_id, status='WON')[0].name
        else:
            return ''

    def get_game_loser(self):
        if Players.objects.filter(game=self.game_id, status='LOST', lost=False).exists():
            return Players.objects.filter(game=self.game_id, status='LOST', lost=False)[0].name
        else:
            return ''

    def get_games_pending(self):
        game_ids = []
        for game in Games.objects.filter(status=self.game_status[0]):
            if not Players.objects.filter(game=game).exists():
                game.delete()
            else:
                game_ids.append(game.id)
        return game_ids

    def get_games_in_progress(self):
        for game in Games.objects.filter(status=self.game_status[1]):
            if not Players.objects.filter(game=game).exists():
                game.delete()
        return Games.objects.filter(status=self.game_status[1]).count()

    def get_player_names(self):
        names = []

        for player in Players.objects.filter(game=self.game_id):
            names.append(player.name)

        return names

    def get_curr_player_turn(self):
        return Players.objects.filter(game=self.game_id, their_turn=True)[0].name

    def get_weapon_locations(self):
        weapon_locations = {}
        for weapon in Weapons.objects.filter(game=self.game_id):
            if weapon.location != None:
                weapon_locations[weapon.name] = weapon.location.name
        return weapon_locations

    def get_suspect_locations(self):
        suspect_locations = {}
        for suspect in Suspects.objects.filter(game=self.game_id):
            if suspect.location != None:
                suspect_locations[suspect.name] = suspect.location.name
            else:
                suspect_locations[suspect.name] = ''
        return suspect_locations

    def delete_completed_games(self):
        if Games.objects.filter(status="COMPLETE").exists():
            Games.objects.filter().delete()

    def remove_player(self, check, client_ip, client_name):
        if check is not '':
            Players.objects.filter(client_ip=client_ip, client_name=client_name).delete()
