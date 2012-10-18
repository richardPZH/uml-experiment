function [ ] = testHierachical2( trVector , trGist , trlabels , teVector , teGist , telabels , hierachin )

% embed all the three level hash and perform:
% 	1.the recall-precision plot
%   2.may plot the original image, show to human
%
% Input :
%     trVector, the nx3072 CIFAR original training image , use for plotting image
%     trGist, the nx320 GIST representation of CIFAR training image;
%     trlabels, the true class of the image , range from 0-9 (CIFAR) training
%     teVector, the nx3072 CIFAR original testing image
%     teGist,  the nx320 Gist representation of CIFAR testing image;
%     telabels, the true class of the image , range from 0-9 (CIFAR) testing
%     hierachin, usually a 3x1 vector, 
%     	hierachin(1) is the bits of the first level
%		hierachin(2) is the bits of the second level
%  		hierachin(3) is the bits of the third level
%
% Output:
%
%
% Author: 
%     IMS@SCUT Once 2012/10/18
%     





