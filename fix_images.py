from PIL import Image
import os

def fix_images_in_directory(directory):
    for person_name in os.listdir(directory):
        person_folder = os.path.join(directory, person_name)

        if not os.path.isdir(person_folder):  # Skip non-folder items
            continue

        for filename in os.listdir(person_folder):
            file_path = os.path.join(person_folder, filename)

            # Only process image files
            if not (filename.endswith(".jpg") or filename.endswith(".png")):
                continue

            try:
                # Try to open the image using PIL
                img = Image.open(file_path)
                
                # Convert image to RGB if it's not
                img = img.convert("RGB")
                
                # Save the corrected image
                img.save(file_path)
                print(f"Fixed and saved image: {file_path}")
            except Exception as e:
                print(f"Error fixing image {file_path}: {e}")

if __name__ == '__main__':
    base_dir = "faces"  # Your folder containing subfolders for each person
    fix_images_in_directory(base_dir)
