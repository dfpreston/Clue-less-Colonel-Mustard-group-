# Generated by Django 2.1b1 on 2019-03-04 23:17

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('Clueless', '0005_cards_solution'),
    ]

    operations = [
        migrations.AddField(
            model_name='players',
            name='status',
            field=models.CharField(default='ACTIVE', max_length=50),
        ),
    ]
