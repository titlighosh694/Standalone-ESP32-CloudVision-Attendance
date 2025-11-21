import cv2
import requests

# Load and resize
img = cv2.imread("test.jpeg")
img = cv2.resize(img, (300, 300))  # Resize to 300x300

_, img_encoded = cv2.imencode('.jpg', img)

# Send to server
response = requests.post("http://localhost:5000/recognize", data=img_encoded.tobytes())
print(response.status_code)
print(response.json())
