function [] = Level1Hash(trData,trLabel,teData,teLabel,method )
% This is a classifier to classify the nonliving and living things
% We can have many classifier and try...
%
% Input:
%      trData, the training data, nxd , n is the number of samples, d is the dimension , On the CIFAR dataset, we use the 320 GIST representation of;
%      trLabel , the class of the samples in trData , on CIFAR 0-9
%      teData, the test sample, use to evaluate the classifier
%      teLabel, the true label of the teData
%
% Author:
%      IMS@SCUT

trData = double( trData );
teData = double( teData );

% The origin CIFAR labels is from 0-9, we use the funciton ChangeLabel to change the living thing to 0 and nonliving thing to 1, so this is 2 class problem
trLabel = ChangeLabel( trLabel , 'vec' );
teLabel = ChangeLabel( teLabel , 'vec' );


trDS = dataset( trData , trLabel );
teDS = dataset( teData , teLabel );

switch method
case 'knnc'
	K = 10;
	classifier = knnc( trDS , K );

case 'linear'

case 'bayes'

end

telabelOutput = labeld( teDS , classifier );
correct = sum( teLabel == telabelOutput );
acc = correct / ( size( teData , 1 ));

disp( 'Accuracy of the test is ' );
disp( acc );



