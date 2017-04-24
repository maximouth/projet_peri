#!/usr/bin/python2

import cgi 
import sqlite3
import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt


form = cgi.FieldStorage()

print("Content-type: text/html; charset=utf-8\n")



html = """<!DOCTYPE html>
<head>
    <title>Mon programme</title>
</head>
<body>

<h1>Afficher le graphe de la luminescence de votre piece </h1>
<img src="/www/img/peri.png" alt="logo" >

    <form action="/bd.py" method="post">
        <input type="text" name="val" value="nouveau temps pour le graph" />
        <input type="submit" name="send" value="generer">
    </form> 
"""

fin = """</body>
</html>
"""

print(html)

lu = form.getvalue("val")

print("valeur du graph : " + lu)

conn = sqlite3.connect('../ma_base.db')

cursor =conn.cursor()

cursor.execute("Select * FROM photores Where id < " + lu * 60 + " ;" ) 

rows = cursor.fetchall()

ids = []
valeur = []

for row in rows:
    print ("<h4>")
    print ('{0} : {1} '.format(row[0], row[1]))
    print ("</h4>")
    ids.append(row[0])
    valeur.append(row[1])
    
conn.close()

plt.plot(ids, valeur)  # on utilise la fonction sinus de Numpy
plt.ylabel('lumiere de la piece')
plt.xlabel("Temps (s)")
plt.savefig('plot',format='png')

print ("<img src=\"/plot.png\" alt=\"plot\" >")

print (fin)
