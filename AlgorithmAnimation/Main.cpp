# include <Siv3D.hpp> // OpenSiv3D v0.3.2

void updateTexture(Image& image, DynamicTexture& texture) {
	texture.fill(image);
	texture.draw();
	System::Update();
}

void repaintImage(Image& image, Array<int32>& arr, const int32& repaintIndex) {
	int32 x = repaintIndex * 2;
	for (int32 y = 0; y < 600; y++) {
		image[y][x] = Color(0, 0, 0);
		image[y][x + 1] = Color(0, 0, 0);
	}
	for (int32 y = 600 - arr[repaintIndex]; y < 600; y++) {
		image[y][x] = Color(255, 255, 255);
		image[y][x + 1] = Color(255, 255, 255);
	}
}

void updateImage(Image& image, DynamicTexture& texture, Array<int32>& arr, const int32& swapedIndexLeft, const int32& swapedIndexRight) {
	repaintImage(image, arr, swapedIndexLeft);
	repaintImage(image, arr, swapedIndexRight);
	updateTexture(image, texture);
}

void paintPivot(Image& image, DynamicTexture& texture, Array<int32>& arr, int32 pivotIndex) {
	int32 x = pivotIndex * 2;
	for (int32 y = 600 - arr[pivotIndex]; y < 600; y++) {
		image[y][x] = Color(255, 0, 0);
		image[y][x + 1] = Color(255, 0, 0);
	}
}

int32 partition(Array<int32>& arr, const int32& left, const int32& right, Image& image, DynamicTexture& texture) {
	const int32 pivot = arr[right];
	paintPivot(image, texture, arr, right);

	int32 pivotIndex = left - 1;
	for (int i = left; i < right; i++) {
		if (arr[i] <= pivot) {
			pivotIndex++;
			std::swap(arr[i], arr[pivotIndex]);

			updateImage(image, texture, arr, i, pivotIndex);
		}
	}
	std::swap(arr[right], arr[++pivotIndex]);
	updateImage(image, texture, arr, right, pivotIndex);

	return pivotIndex;
}

void quickSort(Array<int32>& arr, const int32& left, const int32& right, Image& image, DynamicTexture& texture) {
	if (left >= right) return;

	int32 pivotIndex = partition(arr, left, right, image, texture);

	quickSort(arr, left, pivotIndex - 1, image, texture);
	quickSort(arr, pivotIndex + 1, right, image, texture);
}


void init(Image & image, DynamicTexture texture, Array<int32> & arr) {
	for (int32 k = 0; k < 400; k++) arr[k] = Random<int32>(100, 600);

	image.fill(Palette::Black);
	int32 w = 0;
	for (int32 x = 0; x < 400; x++) {
		for (int32 y = 600 - arr[x]; y < 600; y++) {
			image[y][w] = Color(255, 255, 255);
			image[y][w + 1] = Color(255, 255, 255);
		}
		w += 2;
	}
	updateTexture(image, texture);
}


void Main()
{

	DynamicTexture texture;
	Image image(800, 600);
	Array<int32> arr(400);

	while (System::Update())
	{
		if (KeyR.down())
		{
			init(image, texture, arr);
			quickSort(arr, 0, 400 - 1, image, texture);
		}
		texture.fill(image);
		texture.draw();

	}
}