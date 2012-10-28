#!/bin/bash

# This script lets you present different menus to Tux.  He will only be happy
# when given a fish.  To make it more fun, we added a couple more animals.
export menu="$1"
export animal="$2"
if [ "$menu" == "fish" ]; then
  if [ "$animal" == "penguin" ]; then
    echo -e "Hmmmmmm fish... Tux happy!\n"
  elif [ "$animal" == "dolphin" ]; then
    echo -e "\a\a\aPweetpeettreetppeterdepweet!\a\a\a\n"
  else
    echo -e "*prrrrrrrt*\n"
  fi
else
  if [ "$animal" == "penguin" ]; then
    echo -e "Tux don't like that.  Tux wants fish!\n"
    exit 1
  elif [ "$animal" == "dolphin" ]; then
    echo -e "\a\a\a\a\a\aPweepwishpeeterdepweet!\a\a\a"
    exit 2
  else
    echo -e "Will you read this sign?!  Don't feed the "$animal"s!\n"
    exit 3
  fi
fi
