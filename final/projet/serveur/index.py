#!/usr/bin/python3

import cgi 
import sqlite3

form = cgi.FieldStorage()

html = """<!DOCTYPE html>
<head>
    <title>Mon programme</title>
</head>
<body>

<h1>Afficher le graphe de la luminescence de votre piece </h1>
<img src="/www/img/peri.png" alt="logo" >
    <form action="/bd.py" method="post">
        <input type="text" name="val" value="temps pour le graph" />
        <input type="submit" name="send" value="Envoyer information au serveur">

"""
fin = """
</form>
</body>
</html>
"""

print(html)

conn = sqlite3.connect('../ma_base.db')

cursor =conn.cursor()

cursor.execute ("""
CREATE TABLE IF NOT EXISTS Photoresistance (

    id INTEGER,					
    valeur INT NOT NULL,				
    date TEXT NOT NULL,				
    PRIMARY KEY(id)					

 );
""")
conn.commit()

# users = []
# users.append((0, 10))
# users.append((2, 42))
# users.append((3, 44))
# users.append((4, 9))


# cursor.executemany("""
# INSERT INTO photores(id, val) VALUES(?, ?)""", users)

conn.commit()

conn.close()


print (fin)
