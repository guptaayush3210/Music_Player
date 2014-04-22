#!/bin/bash
cd /host/ubuntu/songs
# alias ls = ls -al --color
# ls -R|grep .mp3$
# echo "$(tput setaf 4)$(ls -R|grep .mp3$)$(tput sgr0)"

# mix[0]=" "
# mix[1]="-"
# c=1
# while [[ $c -lt 100 ]]; do
# 	clear
# 	x=$RANDOM%2
# 	y=$RANDOM%2
# 	z=$RANDOM%2
# 	i=0
# 	str1=""
# 	str2=""
# 	str3=""
# 	while [[ $i -lt 34 ]]; do
# 		str1="$str1${mix[$x]}${mix[$y]}${mix[$z]}"
# 		str2="$str2${mix[$y]}${mix[$z]}${mix[$x]}"
# 		str3="$str3${mix[$z]}${mix[$x]}${mix[$y]}"
# 		i=$i+1
# 	done
# 	echo "$str1"
# 	echo "$str2"
# 	echo "$str3"
# 	echo "$str1"
# 	sleep 0.3
# 	c=$c+1
# 	#statements
# done

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




IFS=%
list=$(zenity --file-selection --filename=/host/ubuntu/songs/ --title="Select Files to Play" --multiple --separator='%')
:>'/home/ashish/Desktop/Music_Player/FILES/text.txt'
for f in  $list; do
        songname=`echo "$f" | awk -F"songs/" '{print $2}'`
        echo "$songname" >> '/home/ashish/Desktop/Music_Player/FILES/text.txt'
        
done
echo "Main Menu" >> '/home/ashish/Desktop/Music_Player/FILES/text.txt'
# mpg123 -Zq $list



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








