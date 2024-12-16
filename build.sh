#!/bin/zsh

# Run xcodebuild
xcodebuild clean build -project "Gyokuro.xcodeproj" -scheme "Gyokuro"

# Check if the build succeeded
if [[ $? -ne 0 ]]; then
  echo "❌ Build failed. Please check the error messages above." >&2
  exit 1
fi
