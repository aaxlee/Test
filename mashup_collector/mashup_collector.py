import os
import requests

def get_songs(base_url, name=None, songID=None, format="sfh", levelID=None, states=None):

  params = {
    "name": name,
    "songID": songID,
    "format": format,
    "levelID": levelID,
    "states": states
  }

  # filter None values
  params = {k : v for k, v, in params.items() if v is not None}

  try:
    response = requests.get(base_url, params=params)

    if response.status_code == 200:
      songs_data = response.json()
      return songs_data
    else:
      print(f"Error: {response.status_code} - {response.text}")
      return None
  except Exception as e:
    print(f"An error occurred: {str(e)}")
    return None

def download_song(song, download_path):
  download_url = song.get("downloadUrl")
  song_id = song.get("songID")
  song_name = song.get("songName")

  if not download_url:
    print(f"No Download URL Available For {song_name}")
  
  target_path = os.path.join(download_path, f"{song_id}.mp3")

  try:
    # Stream the content from the URL
    with requests.get(download_url, stream=True) as response:
      # Check if the request was successful (status code 200)
      if response.status_code == 200:
        # Open the file in write-binary ('wb') mode
        with open(target_path, 'wb') as file:
          # Write the data in chunks to avoid memory overload
          for chunk in response.iter_content(chunk_size=8192):
            if chunk:  # Filter out keep-alive chunks
              file.write(chunk)
          print(f"File downloaded successfully: {target_path}")
      else:
        print(f"Failed to download file. HTTP Status code: {response.status_code}")
  except Exception as e:
    print(f"An error occurred: {str(e)}")


def main():
  base_url = "https://api.songfilehub.com/v2/songs"
  mashup_folder_path = "c++ projects\GD\Mashup Switcher\Mashups"

  song_name = input("Enter Level Name: ")

  songs = get_songs(base_url, name=song_name)
  if songs:
    count = 0
    print("_____AVAILABLE SONGS_____")
    for song in songs:
      print(f"{count}    {song['songName']}")
      count += 1

    print(f"Select Song To Download: 0 - {count - 1}")
    input_index = int(input("->"))
    song_to_download = songs[input_index]
    download_path = os.path.join(mashup_folder_path, song_name, song_to_download.get("songName"))
    os.makedirs(download_path, exist_ok=True)

    download_song(song_to_download, download_path)

  else:
    print("No songs found")

if __name__ == "__main__":
  main()