// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "Rasterizer",
    products: [
        .library(name: "Rasterizer", targets: ["Rasterizer"])
    ],
    targets: [
        .target(
            name: "Rasterizer",
            path: "./src",
            exclude: ["CMakeLists.txt"]
        )
    ]
)