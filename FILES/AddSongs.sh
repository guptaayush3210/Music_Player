cd /host/ubuntu/songs
IFS=%
list=$(zenity --file-selection --filename=/host/ubuntu/songs/ --title="Select Files to Play" --multiple --separator='%')
for f in  $list; do
        # songname=`echo "$f" | awk -F"songs/" '{print $2}'`
        echo "$f" >> '/home/ashish/Desktop/Music_Player/FILES/currentplay.pl'        
done