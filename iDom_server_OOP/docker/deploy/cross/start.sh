

docker build --no-cache -t rpi-crosscompile .

docker run -it --rm -v $(pwd):/workspace rpi-crosscompile