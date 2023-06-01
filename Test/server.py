import cv2
import numpy as np
import socket
import struct

# Define UDP Socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))

while True:
    # Initialize an empty byte array
    stringData = b""
    
    # Receive data in chunks
    while True:
        # Get the size of the data first
        length, _ = sock.recvfrom(16)
        chunk, _ = sock.recvfrom(int(length))
        
        # Append the chunk
        stringData += chunk

        # Break if the chunk is less than the max size, indicating that it's the last chunk
        if len(chunk) < 60000:
            break
    
    # Reconstruct the data as an image
    data = np.fromstring(stringData, dtype='uint8')
    img_decoded = cv2.imdecode(data, cv2.IMREAD_COLOR)
    
    # Display the resulting frame
    cv2.imshow('frame', img_decoded)
    
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cv2.destroyAllWindows()
sock.close()
