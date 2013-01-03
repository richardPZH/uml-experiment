%circlefinder - useage example
clc
close all
clear all

filename = 'mbike2.jpg';
im = imread(filename);

% finds the circles
[r c rad] = circlefinder(im);

% draws the circles
for n=1:length(rad)
    im = RGBCircle(im,r(n),c(n),rad(n), [0 255 0], 2);
end
figure;
imshow(im)