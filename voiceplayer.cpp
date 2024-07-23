#include "voiceplayer.h"

void Voice::init(){
  comm.begin(9600);
  while(!player.begin(comm)){
    Serial.println("Player-failed");
  }
  Serial.println("Player-success");
}

void Voice::listFiles(){
  int fileCount = player.readFileCounts(DFPLAYER_DEVICE_SD);
  Serial.println(fileCount);
}

bool playFile_isPlaying = false;
void Voice::playFile(int ID){
  if(!playFile_isPlaying){
    player.volume(10);
    player.play(ID);
    Serial.println("play here");
  }
  // if(player.available()){
  //   uint8_t status = player.readType();
  //   Serial.println(status);
  //   if(status == DFPlayerPlayFinished){
  //     playFile_isPlaying = false;
  //     Serial.println("finished");
  //   }
  // }
}