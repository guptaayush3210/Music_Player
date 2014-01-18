#!/bin/bash
cd /host/ubuntu/songs
# alias ls = ls -al --color
# ls -R|grep .mp3$
# echo "$(tput setaf 4)$(ls -R|grep .mp3$)$(tput sgr0)"
mix[0]=" "
mix[1]="-"
# FILE=`zenity --file-selection --multiple --filename=/host/ubuntu/songs/ --title="Select a File"`

#         case $? in
#                  0)
#                         echo "\"$FILE\" selected.";;
#                  1)
#                         echo "No file selected.";;
#                 -1)
#                         echo "No file selected.";;
#         esac
# echo "$(shuf -i 0-1 -n 1)"
# read song
IFS=@
for f in $(zenity --file-selection --filename=/host/ubuntu/songs/ --title="Select a File" --multiple --separator='@') ; do
        songname=`echo "$f" | awk -F"songs/" '{print $2}'`
        zenity --info \
          --text="$(echo $(mpg123 -v $songname))"
        # mpg123 -v $songname
done

# "$FILE" | mpg123 -vZc --list -
# while [ "$song" != "" ]
# do
# count=`ls -R|grep "$song"|wc -l`
# if [ $count -eq 1 ]
# then
# 	c=1
# 	# while [ $count -lt 10 ]
# 	# do
# 		# echo ${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}
# 		# echo "$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])"
# 		# echo "$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])"
# 		# echo "$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])$(mix[$(shuf -i 0-1 -n 1)])"
# 		# c=c+1
# 	# done
# 	screen
# 	mpg123 "`ls -R|grep "$song"`"
# 	exit
# 	echo ${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}${mix[$(shuf -i 0-1 -n 1)]}
	

# else
# 	ls -R|grep "$song" 
# 	echo "Choose the song to play from above i.e please elaborate your search"
# 	read song
# fi
# done








