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

print("valeur du graph : " + lu + " min.")

conn = sqlite3.connect('../ma_base.db')

cursor = conn.cursor()

tmp = int(lu) * 6

#print("tmp : " + str(tmp))

cursor.execute("Select * FROM Photoresistance Where id < " + str(tmp) + ";" ) 

rows = cursor.fetchall()

ids = []
valeur = []
temps = []

for row in rows:
    # print ("<h4>row[0] : " + str(row[0]) + "</h4>")
    # print ("<h4>row[1] : " + str(row[1]) + "</h4>")
    # print ("<h4>row[2] : " + str(row[2]) + "</h4>")
    ids.append(row[0])
    valeur.append(row[1])
    temps.append(row[2])
conn.close()

plt.figure(figsize=(15,8))
plt.xticks (ids, temps, rotation=90)

plt.plot(ids, valeur) 

plt.grid (b='on')
plt.ylabel('lumiere de la piece', rotation=45)
plt.xlabel("Temps (s)", rotation=45)
plt.savefig("www/plot.png",format='png' )#, dpi=600)


print ("<img src=\"/www/plot.png\" alt=\"plot\" >")

print (fin)
