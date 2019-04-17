# include <Siv3D.hpp> // OpenSiv3D v0.3.2

class Sort {
private:
	Image m_image;
	Array<int32> m_arr;
	DynamicTexture m_texture;

	void updateTexture() {
		m_texture.fill(m_image);
		m_texture.draw();
		System::Update();
	}

	void repaintImage(const int32& repaintIndex) {
		int32 x = repaintIndex * 2;
		for (int32 y = 0; y < 600; y++) {
			m_image[y][x] = Color(0, 0, 0);
			m_image[y][x + 1] = Color(0, 0, 0);
		}
		for (int32 y = 600 - m_arr[repaintIndex]; y < 600; y++) {
			m_image[y][x] = Color(255, 255, 255);
			m_image[y][x + 1] = Color(255, 255, 255);
		}
	}

	void updateImage(const int32& swapedIndexLeft, const int32& swapedIndexRight) {
		repaintImage(swapedIndexLeft);
		repaintImage(swapedIndexRight);
		updateTexture();
	}

	void paintPivot(int32 pivotIndex) {
		int32 x = pivotIndex * 2;
		for (int32 y = 600 - m_arr[pivotIndex]; y < 600; y++) {
			m_image[y][x] = Color(255, 0, 0);
			m_image[y][x + 1] = Color(255, 0, 0);
		}
	}

public:
	Sort()
		: m_image(800, 600)
		, m_texture(m_image)
		, m_arr(400)
	{
		init();
	}

	void init() {
		for (int32 k = 0; k < 400; k++) m_arr[k] = Random<int32>(100, 600);

		m_image.fill(Palette::Black);
		int32 w = 0;
		for (int32 x = 0; x < 400; x++) {
			for (int32 y = 600 - m_arr[x]; y < 600; y++) {
				m_image[y][w] = Color(255, 255, 255);
				m_image[y][w + 1] = Color(255, 255, 255);
			}
			w += 2;
		}
		updateTexture();
	}

	int32 partition(const int32& left, const int32& right) {
		const int32 pivot = m_arr[right];
		paintPivot(right);

		int32 pivotIndex = left - 1;
		for (int i = left; i < right; i++) {
			if (m_arr[i] <= pivot) {
				pivotIndex++;
				std::swap(m_arr[i], m_arr[pivotIndex]);

				updateImage(i, pivotIndex);
			}
		}
		std::swap(m_arr[right], m_arr[++pivotIndex]);
		updateImage(right, pivotIndex);

		return pivotIndex;
	}


	void quickSort(const int32& left, const int32& right) {
		if (left >= right) return;

		int32 pivotIndex = partition(left, right);

		quickSort(left, pivotIndex - 1);
		quickSort(pivotIndex + 1, right);
	}

	void merge(const int32& left, const int32& mid, const int32& right) {
		const int32 leftSideLength = mid - left;
		const int32 rightSideLength = right - mid;

		Array<int32> leftSide(leftSideLength);
		Array<int32> rightSide(rightSideLength);

		for (int k = 0; k < leftSideLength; k++) {
			leftSide[k] = m_arr[left + k];
		}
		for (int k = 0; k < rightSideLength; k++) {
			rightSide[k] = m_arr[mid + k];
		}

		int l = 0, r = 0;
		for (int k = left; k < right; k++) {
			if (l >= leftSideLength) {
				m_arr[k] = rightSide[r];
				r++;
				continue;
			}
			if (r >= rightSideLength) {
				m_arr[k] = leftSide[l];
				l++;
				continue;
			}

			if (leftSide[l] <= rightSide[r]) {
				m_arr[k] = leftSide[l];
				l++;
			}
			else {
				m_arr[k] = rightSide[r];
				r++;
			}
			repaintImage(k);
			updateTexture();
		}
	}

	void mergeSort(const int32& left, const int32& right) {
		if (right - left > 1) {
			const int32 mid = (left + right) / 2;
			mergeSort(left, mid);
			mergeSort(mid, right);
			merge(left, mid, right);
		}
	}
};

void Main()
{
	constexpr int32 elementCount = 400;
	Sort quickSort;
	Sort margeSort;


	while (System::Update())
	{
		if (KeyR.down())
		{
			//quickSort.init();
			//quickSort.quickSort(0, elementCount - 1);
			margeSort.init();
			margeSort.mergeSort(0, elementCount);
		}

	}
}