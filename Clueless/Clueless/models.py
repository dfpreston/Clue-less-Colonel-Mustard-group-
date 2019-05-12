from django.db import models


class Games(models.Model):
    created_at = models.DateTimeField(auto_now_add=True)
    status = models.CharField(max_length=200, default='')
    turn_order = models.CharField(max_length=200, default='')
    curr_refuter = models.ForeignKey('Players', on_delete=models.CASCADE, null=True)


class Locations(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    type = models.CharField(max_length=200, default='')


class Weapons(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    location = models.ForeignKey('Locations', on_delete=models.CASCADE, null=True)


class Players(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    location = models.ForeignKey('Locations', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    game_creator = models.BooleanField(default=False)
    their_turn = models.BooleanField(default=False)
    client_ip = models.CharField(max_length=200, default='')
    client_name = models.CharField(max_length=200, null=True)
    status = models.CharField(max_length=50, default='ACTIVE')
    moved = models.BooleanField(default=False)
    moved_by_suggest = models.BooleanField(default=False)
    suggested = models.BooleanField(default=False)
    is_curr_refuter = models.BooleanField(default=False)
    is_unable_to_refute = models.BooleanField(default=False)
    lost = models.BooleanField(default=False)


class Cards(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    player = models.ForeignKey('Players', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    card_type = models.CharField(max_length=200, default='')
    used = models.BooleanField(default=False)
    solution = models.BooleanField(default=False)
    suggested = models.BooleanField(default=False)
