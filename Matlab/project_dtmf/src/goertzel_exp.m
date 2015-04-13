% DTMF program

% change the wav file to yourself dir
[y, fs] = wavread( 'C:\usr\root\temp\Matlab\project_dtmf\dtmf_training_files\Button3_4.wav' );

y = y - mean( y );

y = y ./ max( abs(y) );

N = 512;
frame = zeros( 1, N );
frameNum = floor( length( y ) / N );

% DTMF frequency
f = [ 697, 770, 852, 943, 1209, 1336, 1477, 1633 ]';
dtmf = [ ['1', '2', '3', 'a'];
         ['4', '5', '6', 'b'];
         ['7', '8', '9', 'c'];
         ['*', '0', '#', 'd'];
       ];
fNum = length( f );

w = f ./ fs;

coef = 2 .* cos( 2 .* pi .* w );

s = zeros( fNum, N );
X = zeros( fNum, frameNum );

for i = 0 : 1 : frameNum - 1
    frame = y( i * N + 1 : (i + 1) * N );
    
    s( :, 1 ) = frame( 1 );
    s( :, 2 ) = frame( 2 ) + 2 * s( :, 1 ) .* coef;
    for j = 3 : 1 : N
        s( :, j ) = frame( j ) + coef .* s( :, j - 1 ) - s( :, j - 2 );
    end

    X( :, i + 1 ) = s( :, N - 1 ) .^ 2 + s( :, N ) .^ 2 - coef .* s( :, N - 1 ) .* s( :, N );
end

% discriminate digit from dual tone
ave = mean( X, 2 );
variance = var( X, 1, 2 );
stdd = sqrt( variance );

Y = zeros( fNum, frameNum );

%threshold = ave + 1 .* stdd;
threshold = ave;

threshold( stdd < 100 ) = 500;

for i = 1 : 1 : frameNum
    Y( :, i ) = X( :, i ) > threshold;
end

Y( :,sum(Y,1) <= 1 ) = 0;

ref   = [];
count = 0;

%frameNum

number = [];
for i = 1 : 1 : frameNum
    if sum( Y( :, i ) ) == 0
        if ( ~isempty( ref ) && count >= 2 )
            index = find( ref == 1 );
            number = [ number, dtmf( index( 1 ), index( 2 ) - 4 ) ];
        end
        count = 0;
        ref   = [];
    elseif sum( Y( :, i ) ) == 2
        if ~isempty( ref )
            if ~isequal( ref, Y( :, i ) )
                display( 'error' );
            end
        else
            ref = Y( :, i );
        end

        count = count + 1;
    end
end

display( ['The wave has number: ', number] );

