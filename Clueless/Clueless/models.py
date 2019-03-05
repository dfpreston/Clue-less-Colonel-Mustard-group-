from django.db import models


class Games(models.Model):
    created_at = models.DateTimeField(auto_now_add=True)
    status = models.CharField(max_length=200, default='')


class Locations(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    type = models.CharField(max_length=200, default='')


class Players(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    location = models.ForeignKey('Locations', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    game_creator = models.BooleanField(default=False)
    their_turn = models.BooleanField(default=False)
    client_ip = models.CharField(max_length=200, default='')
    client_name = models.CharField(max_length=200, default='')
    status = models.CharField(max_length=50, default='ACTIVE')


class Cards(models.Model):
    game = models.ForeignKey('Games', on_delete=models.CASCADE, null=True)
    player = models.ForeignKey('Players', on_delete=models.CASCADE, null=True)
    name = models.CharField(max_length=200, default='')
    card_type = models.CharField(max_length=200, default='')
    used = models.BooleanField(default=False)
    solution = models.BooleanField(default=False)