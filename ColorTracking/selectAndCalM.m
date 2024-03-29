function [] = selectAndCalM( hdl )

% This is just one possible way to produce the M mapping in paper TOBCA
% Design Pattern
% 
% step 1:
% select the object to trace... 
% may 1)manual select object to trace. 
%     2)click and collect 13x12 points
%     3)just trace red, 14x14 subsampling the image and get approximate red
%
%
% Author:
%       IMS Once@SCUT
%       2012/12/13   Before the Dawn

rect = getrect( hdl );       %hdl should be the preview handle, help getrect

rect