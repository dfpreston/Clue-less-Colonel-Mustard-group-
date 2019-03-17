from Clueless.models import Games, Cards, Players, Locations


class PlayerManager:
    """
    This class will be used to manage each player and will be used
    to update the information of each player
    """

    def __init__(self, client_ip, client_name):
        """
        """
        self.client_ip = client_ip
        self.client_name = client_name
        self.player = Players.objects.filter(client_ip=client_ip, client_name=client_name)[0]
        self.game_id = self.player.game

    def get_player_location(self):
        return self.player.location.name

    def update_player_location(self, location_name):
        new_location = Locations.objects.filter(game=self.game_id, name=location_name)[0]
        self.player.update(location=new_location)

    def update_player_name(self, player_name):
        if player_name is not '':
            self.player.name=player_name
            self.player.save()

    def get_player_name(self):
        return self.player.name

    def get_other_player_locations(self):
        other_locations = {}

        for player in Players.objects.filter(game=self.game_id).exclude(client_ip=self.client_ip, client_name=self.client_name):
            other_locations[player.name] = player.location.name

        return other_locations

    def get_hand(self):
        hand = {}

        for card in Cards.objects.filter(game=self.game_id, player=self.player, used=False):

            if card.card_type not in hand.keys():
                hand[card.card_type] = [card.name]
            else:
                hand[card.card_type].append(card.name)

        return hand

    def get_unused_cards(self):
        available = {}

        for card in Cards.objects.filter(game=self.game_id, used=False):

            if card.card_type not in available.keys():
                available[card.card_type] = [card.name]
            else:
                available[card.card_type].append(card.name)

        return available

    def get_game_id(self):
        return self.game_id.id

    def get_is_creator(self):
        return self.player.game_creator

    def get_is_player_turn(self):
        return self.player.their_turn
