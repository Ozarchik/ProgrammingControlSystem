type data.txt

formatSpec = '%f'
fileID = fopen('data.txt','r');
sizeA = [3 Inf];
A1 = fscanf(fileID,formatSpec,[1 Inf]);
A2 = fscanf(fileID,formatSpec,[2 Inf]);
A3 = fscanf(fileID,formatSpec,[3 Inf]);
A4 = fscanf(fileID,formatSpec,[4 Inf]);
x = uint32(1):uint32(1000);

figure
plot(x,A1)
figure
plot(x,A2)
figure
plot(x,A3)
figure
plot(x,A4)
