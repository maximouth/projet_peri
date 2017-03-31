#!/usr/bin/env python

html="""
<head>
  <title>Peri Web Server</title>
</head>

<body>
VAL LED 0 + VAL LED 1)<br/>
<form method="POST" action="led.py">
  <input name="val" cols="20"></input>
  <input type="submit" value="Entrer">
</form>
</body>

"""

print html
