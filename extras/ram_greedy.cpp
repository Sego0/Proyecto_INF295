float error_cuad_medio(const Canvas& target, const Canvas& canvas) {
    double error = 0.0;
    int n = target.width * target.height;
    for (int i = 0; i < n; ++i) {
        int idx = i * 3;
        float tr = target.rgb[idx];
        float tg = target.rgb[idx + 1];
        float tb = target.rgb[idx + 2];
        float cr = canvas.rgb[idx];
        float cg = canvas.rgb[idx + 1];
        float cb = canvas.rgb[idx + 2];
        float d = (tr - cr) * (tr - cr) + (tg - cg) * (tg - cg) + (tb - cb) * (tb - cb);
        error += d;
    }
    return static_cast<float>(error / n);
}

float mejora_stroke(const Canvas& target, const Canvas& base, const Stroke& stroke) {
    Canvas temp = base;
    float err_antes = error_cuad_medio(target, base);
    render({stroke}, temp);
    float err_despues  = error_cuad_medio(target, temp);
    return err_antes - err_despues;
}


std::vector<Stroke> greedyInit(const Canvas& target, int maxStrokes, int candidates_iteracion) {
    std::cout << "[Greedy] Generando solución inicial con control espacial...\n";
    Canvas canvas(target.width, target.height);
    canvas.clear(255, 255, 255);

    std::vector<Stroke> strokes;

    std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> sizeDist(0.1f, 0.3f);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);

    // ----- Mapa de ocupación -----
    const int GRID = 16;
    std::vector<int> occupancy(GRID * GRID, 0);

    auto getCellIndex = [&](float xr, float yr) {
        int gx = std::min(GRID - 1, std::max(0, int(xr * GRID)));
        int gy = std::min(GRID - 1, std::max(0, int(yr * GRID)));
        return gy * GRID + gx;
    };

    std::uniform_real_distribution<float> relDist(0.0f, 1.0f);
    std::uniform_real_distribution<float> relCol(0.0f, 255.0f);

    float prevError = error_cuad_medio(target, canvas);
    std::cout << "[Greedy] Error inicial: " << prevError << "\n";

    for (int iter = 0; iter < maxStrokes; ++iter) {
        float bestScore = -1e9;
        Stroke bestStroke;
        bool found = false;

        for (int c = 0; c < candidates_iteracion; ++c) {
            float xr = relDist(rng);
            float yr = relDist(rng);

            // penalizar zonas ocupadas
            int cell = getCellIndex(xr, yr);
            float penalidad_ocupacion = -0.1f * occupancy[cell];

            for (size_t t = 0; t < gBrushes.size(); ++t) {
                Stroke s;
                s.x_rel = xr;
                s.y_rel = yr;
                s.size_rel = sizeDist(rng);
                s.rotation_deg = rotDist(rng);
                s.type = t;
                s.r = relCol(rng);
                s.g = relCol(rng);;
                s.b = relCol(rng);;

                float gain = mejora_stroke(target, canvas, s);
                float score = gain + penalidad_ocupacion;

                if (score > bestScore) {
                    bestScore = score;
                    bestStroke = s;
                    found = true;
                }
            }
        }

        render({bestStroke}, canvas);
        strokes.push_back(bestStroke);
        occupancy[getCellIndex(bestStroke.x_rel, bestStroke.y_rel)] += 1;

        float newError = error_cuad_medio(target, canvas);
        prevError = newError;
    }

    std::cout << "[Greedy] Finalizado con " << strokes.size() << " strokes.\n";
    return strokes;
}