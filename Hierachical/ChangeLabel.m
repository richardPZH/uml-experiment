function [ label ] = ChangeLabel( labels , method )
% ChangLabel will change the CIFAR Label
% { airplane automobile ship truck }
% {    0         1        8    9   } ===> { 1 } These are man made no life
% { bird cat deer dog frog horse   }
% {  2    3   4    5   6    7      } ===> { 0 } These are nature, living thing
%
% Input:
%       labels the original labels of the CIFAR e { 0 ~ 9 }
%       method different way to invoke this function:
%       'for' will use for simple the change the lable
%       'vec' will use vector operation instead of 'for'
% 
% Author:
%       IMS@SCUT Once

n = size( labels , 1 );

label = zeros( size( labels ));  %pre allocate the memory

switch method

case 'for'
	for i = 1 : n 
		label( i ) = labels(i) <=1 || labels(i) >= 8;
	end

otherwise 'vec'
	% I have compute that when Y = -8 * x * x + 72 * x - 112
	% This will seperate the 0 and 1 using Y>=0 ->0 ; Y<0 ->1
	% idea like the SVM?
	label = ( labels.^2 ) *-8 + 72 * labels - 112;
	label( label >=0 ) = 0;
	label( label <0  ) = 1;

end



