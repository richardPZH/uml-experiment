function [ ] = ShowCIFAR( X )
% This function will show the Picture of CIFAR
% How the CIFAR is organise is determined on the website
% Input:
%      X the CIFAR Dataset , n x 3072 , n is the number of the picture
%
% Author:
%       IMS@SCUT Once


%check the X dimension
[ n d ] = size( X );