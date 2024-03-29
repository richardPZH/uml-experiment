function [] = startTracking( )

%This is the main m file for my tracking objects by color alone
%Use the Design Pattern to work, like a factory and PIPE so that replacing one part is as easy as a real component.
%
%
% Be patient and Smart, Think a little bit further
% Author:
%       IMS Once@SCUT
%       2012/12/13   Before the Dawn

clear all;  %for security and firuge(1)
clc;        %too

info = imaqhwinfo()              %used to query input camera

winfo = imaqhwinfo( 'winvideo' ) %select camera, different machine may differ, be bold to select your camera.

winfo.DeviceInfo                 %the selected camera info

obj = videoinput( 'winvideo' , 1 , 'YUY2_320x240' ); %parameter may change to get different effect

set( obj , 'ReturnedColorSpace' , 'rgb' );           %we currently use the rgb colorspace, HSL HSV YUV what about it?

himage = preview( obj );
hdl = figure( 1 );

selectAndCalM( hdl );




