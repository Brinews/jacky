# read in data
# make sure that locations.txt is in the current working directory 
L = read.table("C:/usr/root/temp/J/J-Img/src/3Q/locations.txt",sep = " ",col.names = c("PC1","PC2"))

# plot data
plot(L$PC1,L$PC2,xlab = "Principal component 1",ylab = "Principal component 2",pch = 20,main = "Position of individuals along first two principal components")

# copy plot to pdf
dev.copy2pdf(file = "mySNPplot.pdf")
