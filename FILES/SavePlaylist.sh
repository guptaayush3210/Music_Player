cd /home/ashish/Desktop/Music_Player/FILES/Playlists/
name=$(zenity --entry --entry-text="Playlist" --title="Like It?" --text="Save Playlist As:")
name="$name.txt"
cp /home/ashish/Desktop/Music_Player/FILES/currentplay.txt $name