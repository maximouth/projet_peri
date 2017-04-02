#manque les bon import

import sqlite3

# initialisation nrf24
# pipes = [[0xe7, 0xe7, 0xe7, 0xe7, 0xe7], [0xc2, 0xc2, 0xc2, 0xc2, 0xc2]]

# radio = NRF24()
# radio.begin(0, 0, 17)

# radio.setRetries(15,15)

# radio.setPayloadSize(32)
# radio.setChannel(0x60)
# radio.setDataRate(NRF24.BR_2MBPS)
# radio.setPALevel(NRF24.PA_MIN)

# radio.setAutoAck(True)
# radio.enableDynamicPayloads()
# radio.enableAckPayload()

# radio.openWritingPipe(pipes[0])
# radio.openReadingPipe(1, pipes[1])

# radio.startListening()


# initialisation base de donnees
conn = sqlite3.connect('../ma_base.db')

cursor =conn.cursor()

cursor.execute("""
DROP TABLE If EXISTS photores
""")
conn.commit()

cursor.execute ("""
CREATE TABLE IF NOT EXISTS photores (

   id  INT PRIMARY KEY NOT NULL,
   val INT NOT NULL
 );
""")
conn.commit()



cpt = 0

#remplissage de bd
while True:
    recv_buffer = [2]
#    radio.read(recv_buffer, radio.getDynamicPayloadSize())
    print (recv_buffer)
    request = "Insert Into photores (id, val) Values (" + str(cpt) + ", " + str(recv_buffer[0]) + ");"
    cpt = cpt + 1
    print (request)
    cursor.execute (request)
    conn.commit()
