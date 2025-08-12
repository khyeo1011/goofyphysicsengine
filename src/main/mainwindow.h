#include <iostream>

class MainWindow {
public:
    MainWindow();
    ~MainWindow();

    void show();
    void close();

private:
    // Add your member variables here
    bool isOpen;
};

// Implementation

MainWindow::MainWindow() : isOpen(false) {
    // Initialization code here
}

MainWindow::~MainWindow() {
    // Cleanup code here
}

void MainWindow::show() {
    isOpen = true;
    std::cout << "MainWindow shown." << std::endl;
}

void MainWindow::close() {
    isOpen = false;
    std::cout << "MainWindow closed." << std::endl;
}