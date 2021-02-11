// swift-tools-version:5.3
import PackageDescription

let package = Package(
    name: "rasterizer",
    products: [
        .library(name: "rasterizer", targets: ["rasterizer"])
    ],
    targets: [
        .target(
            name: "rasterizer",
            path: "./Sources/"
        )
    ]
)