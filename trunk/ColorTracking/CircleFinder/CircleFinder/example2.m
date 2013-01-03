%circlefinder - useage example
clc
close all
clear all

filename = 'circle3.jpg';
im = imread(filename);

% finds the circles
[r c rad] = circlefinder(im, [], [], [], [], im);

hold on;