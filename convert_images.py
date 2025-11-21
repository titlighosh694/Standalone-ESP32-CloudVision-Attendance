from PIL import Image
import os

folder_path = "faces"  # Relative path to your face images folder

for subdir, dirs, files in os.walk(folder_path):
    for file in files:
        if file.lower().endswith(('.jpg', '.jpeg', '.png')):
            file_path = os.path.join(subdir, file)
            try:
                img = Image.open(file_path)
                rgb_img = img.convert('RGB')  # Convert to 8-bit RGB
                rgb_img.save(file_path)
            except Exception as e:
                print(f"Failed to convert {file_path}: {e}")
