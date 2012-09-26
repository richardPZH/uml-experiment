function [ recall precision ] = searchImage( inGist , inLabel , inVector , trGist , trLabels , trVector , W0 , R0 , cP0 , E1 , E2 , E3 )
%
% This function use the testing image to evaluate the performance 
% of the Hierachical hashing method... 
%
% Input:
%     inGist, nx320 CIFAR Gist image representation, n images to be evaluate
%	  inLabel, nx1, ground true label of the image; 
%     inVector, the nx3072 original image, can use to display images to human
%     trGist, mx320 training Gist image representation
%     trLabels, 
%     trVector,
%     W0, R0 , cP0 , first level projection , orthogonal rotation , centerPoint
%     E1, the first level entrance
%     E2, the second level entrance
%     E3, the third level entrance
%
% Output:
%     recall: average recall of all the n images
%     precision: average precision of ...
%
% Authors:
%     IMS@SCUT Once 
% Advices:
%     How to use more vector operation rather than for loop















