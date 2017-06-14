cmake_minimum_required(VERSION 2.8.4)
project(musicplayer.hivelyplugin)

set(SOURCE_FILES hively_plugin.c hvl_replay.c)

add_library(hivelyplugin SHARED ${SOURCE_FILES})

