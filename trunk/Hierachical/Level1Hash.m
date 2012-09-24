function [ classifier ] = Level1Hash(trData,trLabel,teData,teLabel,method )
% This is a classifier to classify the nonliving and living things
% We can have many classifier and try... She suggest me to try the
% RandomForest and the MCS
%
% Input:
%      trData, the training data, nxd , n is the number of samples, d is the dimension , On the CIFAR dataset, we use the 320 GIST representation of;
%      trLabel , the class of the samples in trData , on CIFAR 0-9
%      teData, the test sample, use to evaluate the classifier
%      teLabel, the true label of the teData, from 0-9
%
% Output:
%      A classifier that can classify level 1 object into 0 and 1
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

case 'fisherc'
	% May set parameters
	classifier = fisherc( trDS );

case 'perlc'
	% This is a linear classifier too, many parameters can be set
	% help perlc
	classifier = perlc( trDS );

case 'bayes'
	[ dmean , covariance ] = meancov( trDS );
	classifier = nbayesc( dmean , covariance );

case 'bpxnc'
	[ classifier historyReport ] = bpxnc( trDS , [320 2 1] , 100 );

case 'rbnc'
	classifier = rbnc( trDS , 2 );

case 'svc'
    [classifier , J ] = SVC( trDS , proxm([], 'r') );


end

telabelOutput = labeld( teDS , classifier );
correct = sum( teLabel == telabelOutput );
acc = correct / ( size( teData , 1 ));

disp( 'Accuracy of the test is ' );
disp( acc );



