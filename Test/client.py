import cv2
import numpy as np
import socket
import struct

# Define UDP Socket
UDP_IP = "127.0.0.1"
UDP_PORT = 5005

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Open video file or capture from device
cap = cv2.VideoCapture(0)

while(cap.isOpened()):
    ret, frame = cap.read()
    if ret:
        # Encode image as JPEG
        _, img_encoded = cv2.imencode('.jpg', frame)
        data = np.array(img_encoded)
        stringData = data.tobytes()
        
        # Split data into smaller chunks and send
        n = 60000  # Chunk size
        chunks = [stringData[i:i+n] for i in range(0, len(stringData), n)]
        
        for i, chunk in enumerate(chunks):
            # First send the size of the data
            sock.sendto(str(len(chunk)).ljust(16).encode(), (UDP_IP, UDP_PORT))
            # Then the chunk
            sock.sendto(chunk, (UDP_IP, UDP_PORT))
        
        # Just to limit the fps
        cv2.waitKey(30)

cap.release()
sock.close()
