function [ tTTimeElasped time2classifeOneSample Xaccuracy Taccuracy] = DTwithIGwithPessimisticPruncing( precent_of_training )

load yeast.out





%1~8 is attribute 9 is class label
DS = dataset( yeast(:,1:8) , yeast(:,9));

s = size( yeast);
s = s(1);

%disp( s );
%disp( l );

%training sample dataset TS
%testing  sample dataset XS
[ TS XS ] = gendat( DS , precent_of_training * s );
s = size( XS.labels );
s = s(1);

%disp( ts.data );
%disp( xs.data );

%disp( 'Disping full dataset' );
%figure( 1 );
%scatterd(DS);
%title( 'Full DataSet' );

%disp( 'Disping training dataset');
%figure( 2 );
%scatterd(TS);
%title( 'Training DataSet');

%disp( 'Disping testing set');
%figure( 3 );
%scatterd(XS);
%title('Testing DataSet');

    tStart = tic;
    classifier = TREEC( TS , 'infcrit' , 5 );                  %train a tree classifier with Information Gain Prune in used
    tTTimeElasped = toc( tStart );
    
    tStart = tic;
    Xoutput = labeld( XS , classifier);      %get the teXting result
    tXTimeElasped = toc( tStart );
    time2classifeOneSample = tXTimeElasped / s;
    
    len = size( Xoutput );
    len = len( 1 );
    j = 0;
    for i = 1:len
        if( Xoutput(i) == XS.labels(i) )
            j = j + 1;
        end
    end
           
    Xaccuracy = j * 1.0 / len;
    
    tStart = tic;
    Toutput = labeld( TS , classifier);      %get the Training resullt
    tSTimeElasped = toc( tStart );
    
    len = size( Toutput );
    len = len(1);
    j = 0;
    for i = 1:len
        if( Toutput(i) == TS.labels(i) )
            j = j + 1;
        end
    end
           
    Taccuracy = j * 1.0 / len;

end