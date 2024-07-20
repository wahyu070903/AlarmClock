#include "voiceplayer.h"

void Voice::init(){
  comm.begin(9600);
  while(!player.begin(comm)){
    Serial.println("Player-failed");
  }
}

void Voice::listFiles(){
  int fileCount = player.readFileCounts(DFPLAYER_DEVICE_SD);
  Serial.println(fileCount);
}

bool playFile_isPlaying = false;
void Voice::playFile(int ID){
  if(!playFile_isPlaying){
    player.volume(30);
    player.play(1);
    playFile_isPlaying = true;
  }
}