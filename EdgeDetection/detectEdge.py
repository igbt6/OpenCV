import cv2
from matplotlib import pyplot as plt

input = cv2.imread('aaa.jpg',)
gray = cv2.cvtColor(input, cv2.COLOR_RGB2GRAY)
img = cv2.GaussianBlur(gray,(3,3),0)
laplacian = cv2.Laplacian(img,cv2.CV_64F)
sobel_x = cv2.Sobel(img,cv2.CV_64F,1,0,ksize=5)
sobel_y = cv2.Sobel(img,cv2.CV_64F,0,1,ksize=5)
canny = cv2.Canny(img,4000, 500,apertureSize=5) 
scharr_x = cv2.Scharr(img ,cv2.CV_16S, 1, 0, cv2.BORDER_DEFAULT)
scharr_y = cv2.Scharr(img ,cv2.CV_16S, 0, 1, cv2.BORDER_DEFAULT)

plt.subplot(3,2,1),plt.imshow(img,cmap = 'gray')
plt.title('Original'), plt.xticks([]), plt.yticks([])
plt.subplot(3,2,2),plt.imshow(laplacian, cmap = 'gray')
plt.title('Laplacian'), plt.xticks([]), plt.yticks([])
plt.subplot(3,2,3),plt.imshow(sobel_x, cmap = 'gray')
plt.title('Sobel_x'), plt.xticks([]), plt.yticks([])
plt.subplot(3,2,4),plt.imshow(sobel_y, cmap = 'gray')
plt.title('Sobel_y'), plt.xticks([]), plt.yticks([])
plt.subplot(3,2,5),plt.imshow(canny, cmap = 'gray')
plt.title('Canny'), plt.xticks([]), plt.yticks([])
plt.subplot(3,2,6),plt.imshow(scharr_x, cmap = 'gray')
plt.title('Scharr_x'), plt.xticks([]), plt.yticks([])

plt.show()