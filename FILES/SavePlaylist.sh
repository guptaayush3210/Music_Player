cd /home/ayush/Linux_Project/FILES/Playlists/
name=$(zenity --entry --entry-text="Playlist" --title="Like It?" --text="Save Playlist As:")
name="$name.txt"
cp /home/ayush/Linux_Project/FILES/currentplay.txt $name