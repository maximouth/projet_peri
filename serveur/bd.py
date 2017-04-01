#!/usr/bin/python3

import cgi 
import sqlite3
import plotly.plotly as py
import plotly.graph_objs as go

#plotly.tools.set_credentials_file(username='maximouth', api_key='mhxPshAzUwSAThBSdXpU')

form = cgi.FieldStorage()

print("Content-type: text/html; charset=utf-8\n")



html = """<!DOCTYPE html>
<head>
    <title>Mon programme</title>
</head>
<body>

<h1>Afficher le graphe de la luminescence de votre piece </h1>
<img src="/img/peri.png" alt="logo" >

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

conn = sqlite3.connect('ma_base.db')

cursor =conn.cursor()

cursor.execute("Select * FROM photores Where id < 100" )

rows = cursor.fetchall()

ids = []
valeur = []

for row in rows:
    print ("<h4>")
    print ('{0} : {1} '.format(row[0], row[1]))
    print ("</h4>")
    ids.append((row[0]))
    valeur.append((row[1]))
    
conn.close()

trace = go.Scatter(
    x = ids,
    y = valeur,
    mode = 'lines+markers',
    name = 'lines+markers'
)

data = [trace]

image = "<img src=%s' %(py.plot(data, filename='test'))"

print (image)

print (fin)
