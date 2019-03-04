# Clue-less-Colonel-Mustard-group-
Foundations of Software Engineering Spring 2019


How to run the web server locally:<br><br>
    1) Make sure you have python and django installed (pip install django in commandline)<br><br>
    2) In your terminal, make sure that you are in the "Clueless" directory<br>
        You must be in the same dir as the "manage.py" file<br><br>
    3) Make sure that your db is synced<br>
        - Run the command "python manage.py makemigrations --dry-run" to test DB changes<br>
        - Run the command "python manage.py makemigrations" to confirm DB changes<br>
        - Run the command "python manage.py migrate" to change DB<br><br>
    3) Run the command "python manage.py runserver" in order to run the server<br><br>
    4) In your web browser<br>
