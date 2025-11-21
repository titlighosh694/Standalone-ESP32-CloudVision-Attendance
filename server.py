import numpy as np
import cv2
import os
from flask import Flask, request, jsonify
import face_recognition
from PIL import Image

app = Flask(__name__)

# Preload known face encodings and their names
known_face_encodings = []
known_face_names = []

# Load known faces from folders
def load_known_faces():
    base_dir = "faces"

    for person_name in os.listdir(base_dir):
        person_folder = os.path.join(base_dir, person_name)
        if not os.path.isdir(person_folder):
            continue

        for filename in os.listdir(person_folder):
            file_path = os.path.join(person_folder, filename)
            if not filename.lower().endswith((".jpg", ".jpeg", ".png")):
                continue

            try:
                pil_image = Image.open(file_path).convert('RGB')
                image = np.array(pil_image)

                # Optional: Resize image to reduce noise
                image = cv2.resize(image, (300, 300))

                encodings = face_recognition.face_encodings(image)
                if encodings:
                    known_face_encodings.append(encodings[0])
                    known_face_names.append(person_name)
            except Exception as e:
                print(f"Error processing {file_path}: {e}")

# Load known faces at startup
load_known_faces()

@app.route('/')
def index():
    return 'Face Recognition Flask Server is Running!'

@app.route('/recognize', methods=['POST'])
def recognize_face():
    img_data = request.data
    np_arr = np.frombuffer(img_data, np.uint8)
    img = cv2.imdecode(np_arr, cv2.IMREAD_COLOR)

    # Save the received image for inspection
    cv2.imwrite("received_from_esp32.jpg", img)

    if img is None:
        return jsonify({"error": "Unable to process image"}), 400

    # Flip the image if it's vertically flipped
    img = cv2.flip(img, 0)  # Flip the image vertically

    # Optional: Apply Gaussian blur or resize image for better quality
    img = cv2.GaussianBlur(img, (5, 5), 0)
    img = cv2.resize(img, (640, 480))

    img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)

    try:
        face_locations = face_recognition.face_locations(img)
        face_encodings = face_recognition.face_encodings(img, face_locations)
    except Exception as e:
        return jsonify({"error": f"Face recognition failed: {str(e)}"}), 500

    names = []
    for encoding in face_encodings:
        distances = face_recognition.face_distance(known_face_encodings, encoding)
        if len(distances) == 0:
            names.append("Unknown")
            continue

        best_match_index = np.argmin(distances)
        if distances[best_match_index] < 0.45:
            name = known_face_names[best_match_index]
        else:
            name = "Unknown"

        names.append(name)

    print("Recognized:", names)
    return jsonify({"names": names})

@app.route('/')
def index():
    return "Flask Face Recognition Server is Running"

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
