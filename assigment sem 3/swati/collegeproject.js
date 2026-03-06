import React, { useRef, useState, useEffect } from "react";

/**
 * Virtual Wardrobe & Makeover (College Project Prototype)
 * ------------------------------------------------------
 * What this does (MVP):
 * 1) Upload a selfie (base image).
 * 2) Upload your clothes (tops / bottoms / dresses) and manage a wardrobe list.
 * 3) Try-on screen where you can overlay: a clothing item, a hairstyle, and simple makeup filters.
 * 4) Drag / scale / rotate overlays to roughly align with your face/body.
 * 5) Save looks (as snapshots) and avoid repeating the same combination.
 * 6) Random suggestion from your existing wardrobe to prevent repeating looks.
 *
 * NOTE (for viva/discussion):
 * - This is a front-end only prototype (no server). Images are stored in memory.
 * - Background removal / body/face landmarking is not implemented here to keep it simple for college submission.
 *   In your report, mention future scope: integrate Google MediaPipe (FaceMesh / Pose) or a background-removal API.
 * - The goal is to demonstrate core UX and logic: scan/upload wardrobe, virtual try-on, basic recommendations.
 *
 * How to use in class:
 * - Upload a selfie on the "Profile" tab.
 * - Add clothes on the "Wardrobe" tab (upload PNG/JPG + give it a name + select a category).
 * - Go to "Try On" tab, pick a top/bottom/dress, a hairstyle (preloaded), apply makeup filters and adjust overlays.
 * - Click "Save Look" to capture a snapshot and store it under "Saved Looks".
 */

// ------------------------------
// Utility helpers
// ------------------------------
const fileToDataURL = (file) =>
  new Promise((res, rej) => {
    const reader = new FileReader();
    reader.onload = () => res(reader.result);
    reader.onerror = rej;
    reader.readAsDataURL(file);
  });

const uid = () => Math.random().toString(36).slice(2, 10);

// Simple deep compare for look duplicates (same clothing+hairstyle+filters)
const isSameLook = (a, b) => {
  if (!a || !b) return false;
  return (
    a.topId === b.topId &&
    a.bottomId === b.bottomId &&
    a.dressId === b.dressId &&
    a.hairId === b.hairId &&
    JSON.stringify(a.filters) === JSON.stringify(b.filters)
  );
};

// Some preloaded hairstyles (transparent PNG URLs). For offline demo, we embed tiny base64 svgs.
// You can replace these with better PNGs in your submission assets folder.
const PRELOADED_HAIRSTYLES = [
  {
    id: "hair-bob",
    name: "Classic Bob",
    // Simple SVG hair placeholder; replace with a PNG URL for realism.
    src:
      "data:image/svg+xml;base64,PHN2ZyB3aWR0aD0nNTAwJyBoZWlnaHQ9JzQwMCcgdmlld0JveD0nMCAwIDUwMCA0MDAnIHhtbG5zPSdodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2Zyc+PGc+PGVsbGlwc2Ugcng9JzIwMCcgcj0nMjAwJyBjeD0nMjUwJyBjeT0nMjAwJyBmaWxsPSdibGFjaycvPjxlbGxpcHNlIHJ4PScxNScgY3g9JzI1MCcgY3k9JzE1MCcgcj0nOTAnIGZpbGw9J2Jyb3duJy8+PC9nPjwvc3ZnPiIs",
  },
  {
    id: "hair-ponytail",
    name: "High Ponytail",
    src:
      "data:image/svg+xml;base64,PHN2ZyB3aWR0aD0nNTAwJyBoZWlnaHQ9JzQwMCcgdmlld0JveD0nMCAwIDUwMCA0MDAnIHhtbG5zPSdodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2Zyc+PGNpcmNsZSBjeD0nMjUwJyBjeT0nMTYwJyByPScxMDAnIGZpbGw9J2Jyb3duJy8+PHBhdGggZD0nTTI1MCAxNjAgcTMwIDgwIDEyMCAxNTAiIHN0cm9rZT0nYmxhY2snIGZpbGw9J2Jyb3duJy8+PC9zdmc+",
  },
  {
    id: "hair-curly",
    name: "Curly Volume",
    src:
      "data:image/svg+xml;base64,PHN2ZyB3aWR0aD0nNTAwJyBoZWlnaHQ9JzQwMCcgdmlld0JveD0nMCAwIDUwMCA0MDAnIHhtbG5zPSdodHRwOi8vd3d3LnczLm9yZy8yMDAwL3N2Zyc+PHBhdGggZD0nTTUwIDIzMHE2MC0xNDAgMzAwLTE0MHMxOTAgMTIwIDExMCAyMDBUNTAwIDM1MCAyNDAgMzYwIDUwIDIzMCcgZmlsbD0nYnJvd24nLz48L3N2Zz4=",
  },
];

// ------------------------------
// Main App
// ------------------------------
export default function App() {
  // Profile (selfie/base)
  const [baseImg, setBaseImg] = useState(null);

  // Wardrobe state
  const [wardrobe, setWardrobe] = useState([]); // {id, name, category, src}

  // Current selections for try-on
  const [selectedTop, setSelectedTop] = useState(null);
  const [selectedBottom, setSelectedBottom] = useState(null);
  const [selectedDress, setSelectedDress] = useState(null);
  const [selectedHair, setSelectedHair] = useState(null);

  // Overlay transforms (scale/rotation/position)
  const [overlays, setOverlays] = useState({
    top: { x: 50, y: 50, scale: 1, rot: 0 },
    bottom: { x: 50, y: 220, scale: 1, rot: 0 },
    dress: { x: 50, y: 80, scale: 1, rot: 0 },
    hair: { x: 80, y: 0, scale: 1, rot: 0 },
  });

  // Makeup filters (simple global filters + blush overlays)
  const [filters, setFilters] = useState({
    brightness: 100,
    contrast: 100,
    saturation: 100,
    blur: 0,
    blushLeft: { visible: false, x: 160, y: 180, r: 18, opacity: 0.2 },
    blushRight: { visible: false, x: 260, y: 180, r: 18, opacity: 0.2 },
  });

  // Saved looks (snapshots and metadata)
  const [savedLooks, setSavedLooks] = useState([]); // {id, dataURL, meta}

  // UI state
  const [tab, setTab] = useState("profile");
  const stageRef = useRef(null);

  // --------------- Handlers ---------------
  const handleBaseUpload = async (e) => {
    const file = e.target.files?.[0];
    if (!file) return;
    const url = await fileToDataURL(file);
    setBaseImg(url);
  };

  const handleAddCloth = async (e, category, nameInputId) => {
    const file = e.target.files?.[0];
    if (!file) return;
    const nameEl = document.getElementById(nameInputId);
    const name = nameEl?.value?.trim() || file.name;
    const src = await fileToDataURL(file);
    const item = { id: uid(), name, category, src };
    setWardrobe((prev) => [item, ...prev]);
    if (nameEl) nameEl.value = "";
    e.target.value = ""; // reset
  };

  const itemsByCat = (cat) => wardrobe.filter((w) => w.category === cat);

  const choose = (type, item) => {
    if (type === "top") setSelectedTop(item);
    if (type === "bottom") setSelectedBottom(item);
    if (type === "dress") setSelectedDress(item);
  };

  const chooseHair = (h) => setSelectedHair(h);

  // Dragging overlays: on mouse down track move; simplified for desktop demo
  const startDrag = (key, e) => {
    const startX = e.clientX;
    const startY = e.clientY;
    e.preventDefault();

    const onMove = (ev) => {
      setOverlays((ov) => ({
        ...ov,
        [key]: {
          ...ov[key],
          x: ov[key].x + (ev.clientX - startX),
          y: ov[key].y + (ev.clientY - startY),
        },
      }));
    };
    const onUp = () => {
      window.removeEventListener("mousemove", onMove);
      window.removeEventListener("mouseup", onUp);
    };
    window.addEventListener("mousemove", onMove);
    window.addEventListener("mouseup", onUp);
  };

  const adjust = (key, field, value) => {
    setOverlays((ov) => ({
      ...ov,
      [key]: { ...ov[key], [field]: value },
    }));
  };

  // Build look metadata for duplicate detection
  const currentLookMeta = () => ({
    topId: selectedTop?.id || null,
    bottomId: selectedBottom?.id || null,
    dressId: selectedDress?.id || null,
    hairId: selectedHair?.id || null,
    filters: {
      brightness: filters.brightness,
      contrast: filters.contrast,
      saturation: filters.saturation,
      blur: filters.blur,
      blushLeft: { ...filters.blushLeft, x: undefined, y: undefined }, // ignore position for duplicate check
      blushRight: { ...filters.blushRight, x: undefined, y: undefined },
    },
  });

  // Suggest a non-repeating outfit
  const suggestOutfit = () => {
    const tops = itemsByCat("top");
    const bottoms = itemsByCat("bottom");
    const dresses = itemsByCat("dress");
    if (dresses.length > 0 && Math.random() < 0.4) {
      const d = dresses[Math.floor(Math.random() * dresses.length)];
      setSelectedDress(d);
      setSelectedTop(null);
      setSelectedBottom(null);
    } else if (tops.length && bottoms.length) {
      setSelectedDress(null);
      const t = tops[Math.floor(Math.random() * tops.length)];
      const b = bottoms[Math.floor(Math.random() * bottoms.length)];
      setSelectedTop(t);
      setSelectedBottom(b);
    }
  };

  // Save a snapshot of the stage
  const saveLook = async () => {
    if (!baseImg) return alert("Upload a selfie first on the Profile tab.");

    // Duplicate prevention: if latest saved equals current meta, block
    const meta = currentLookMeta();
    const last = savedLooks[0]?.meta;
    if (last && isSameLook(last, meta)) {
      alert("This look matches your last saved look. Try tweaking items/filters.");
      return;
    }

    // Render to a temporary canvas
    const stage = stageRef.current;
    const rect = stage.getBoundingClientRect();
    const w = rect.width;
    const h = rect.height;
    const canvas = document.createElement("canvas");
    canvas.width = w;
    canvas.height = h;
    const ctx = canvas.getContext("2d");

    // Draw base with filters (CSS filters don't apply to canvas; we replicate basic ones)
    const base = await loadImage(baseImg);
    // Apply basic filters manually (approx): we’ll draw image then run pixel ops for brightness/contrast/saturation
    ctx.drawImage(base, 0, 0, w, h);
    applyCanvasFilters(ctx, w, h, filters);

    // Helper to draw overlay image with transforms
    const drawOverlay = async (imgSrc, o) => {
      const img = await loadImage(imgSrc);
      const dw = img.width * o.scale * 0.5; // scale down a bit for usability
      const dh = img.height * o.scale * 0.5;
      ctx.save();
      ctx.translate(o.x + dw / 2, o.y + dh / 2);
      ctx.rotate((o.rot * Math.PI) / 180);
      ctx.drawImage(img, -dw / 2, -dh / 2, dw, dh);
      ctx.restore();
    };

    if (selectedDress) await drawOverlay(selectedDress.src, overlays.dress);
    if (selectedTop) await drawOverlay(selectedTop.src, overlays.top);
    if (selectedBottom) await drawOverlay(selectedBottom.src, overlays.bottom);
    if (selectedHair) await drawOverlay(selectedHair.src, overlays.hair);

    // Draw blush circles
    const drawBlush = (b) => {
      if (!b.visible) return;
      ctx.save();
      ctx.globalAlpha = b.opacity;
      ctx.beginPath();
      ctx.arc(b.x, b.y, b.r, 0, Math.PI * 2);
      ctx.fillStyle = "#ff3366";
      ctx.fill();
      ctx.restore();
    };
    drawBlush(filters.blushLeft);
    drawBlush(filters.blushRight);

    const dataURL = canvas.toDataURL("image/png");
    setSavedLooks((s) => [{ id: uid(), dataURL, meta }, ...s]);
    alert("Look saved!");
  };

  // Suggest on first entry into Try-On if nothing selected
  useEffect(() => {
    if (tab === "tryon" && !selectedTop && !selectedBottom && !selectedDress) {
      suggestOutfit();
    }
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [tab]);

  return (
    <div className="min-h-screen w-full bg-gray-50 text-gray-900">
      <header className="sticky top-0 z-10 bg-white/80 backdrop-blur border-b">
        <div className="max-w-6xl mx-auto flex items-center justify-between p-4">
          <h1 className="text-2xl font-bold">Virtual Wardrobe & Makeover</h1>
          <nav className="flex gap-2">
            {[
              { id: "profile", label: "Profile" },
              { id: "wardrobe", label: "Wardrobe" },
              { id: "tryon", label: "Try On" },
              { id: "saved", label: "Saved Looks" },
            ].map((t) => (
              <button
                key={t.id}
                onClick={() => setTab(t.id)}
                className={`px-3 py-1.5 rounded-2xl border text-sm ${
                  tab === t.id ? "bg-black text-white" : "bg-white hover:bg-gray-100"
                }`}
              >
                {t.label}
              </button>
            ))}
          </nav>
        </div>
      </header>

      <main className="max-w-6xl mx-auto p-4">
        {tab === "profile" && (
          <section className="grid md:grid-cols-2 gap-6">
            <div className="p-4 bg-white rounded-2xl shadow">
              <h2 className="font-semibold text-lg mb-2">Upload your selfie</h2>
              <input type="file" accept="image/*" onChange={handleBaseUpload} />
              <p className="text-sm text-gray-600 mt-2">
                Tip: Use a front-facing photo with good light. For best try-on, keep head+torso visible.
              </p>
            </div>
            <div className="p-4 bg-white rounded-2xl shadow">
              <h2 className="font-semibold text-lg mb-2">Preview</h2>
              <Stage baseImg={baseImg} overlays={overlays} filters={filters} selected={{ selectedTop, selectedBottom, selectedDress, selectedHair }} stageRef={stageRef} readonly />
            </div>
          </section>
        )}

        {tab === "wardrobe" && (
          <section className="grid lg:grid-cols-3 gap-6">
            <AddClothCard
              title="Add Top"
              category="top"
              inputId="name-top"
              onFile={(e) => handleAddCloth(e, "top", "name-top")}
            />
            <AddClothCard
              title="Add Bottom"
              category="bottom"
              inputId="name-bottom"
              onFile={(e) => handleAddCloth(e, "bottom", "name-bottom")}
            />
            <AddClothCard
              title="Add Dress"
              category="dress"
              inputId="name-dress"
              onFile={(e) => handleAddCloth(e, "dress", "name-dress")}
            />

            <div className="lg:col-span-3 p-4 bg-white rounded-2xl shadow">
              <h3 className="font-semibold mb-3">Your Wardrobe</h3>
              <WardrobeGrid items={wardrobe} onRemove={(id) => setWardrobe((w) => w.filter((x) => x.id !== id))} />
            </div>
          </section>
        )}

        {tab === "tryon" && (
          <section className="grid lg:grid-cols-3 gap-6">
            {/* Left: Controls */}
            <div className="p-4 bg-white rounded-2xl shadow space-y-6">
              <h2 className="font-semibold text-lg">Pick Items</h2>

              {/* Clothing pickers */}
              <Picker
                label="Top"
                items={itemsByCat("top")}
                selected={selectedTop?.id}
                onSelect={(id) => choose("top", itemsByCat("top").find((i) => i.id === id) || null)}
                onClear={() => setSelectedTop(null)}
              />
              <Picker
                label="Bottom"
                items={itemsByCat("bottom")}
                selected={selectedBottom?.id}
                onSelect={(id) =>
                  choose("bottom", itemsByCat("bottom").find((i) => i.id === id) || null)
                }
                onClear={() => setSelectedBottom(null)}
              />
              <Picker
                label="Dress"
                items={itemsByCat("dress")}
                selected={selectedDress?.id}
                onSelect={(id) => choose("dress", itemsByCat("dress").find((i) => i.id === id) || null)}
                onClear={() => setSelectedDress(null)}
              />

              {/* Hair */}
              <div>
                <label className="block font-medium mb-1">Hairstyle</label>
                <div className="flex flex-wrap gap-2">
                  {PRELOADED_HAIRSTYLES.map((h) => (
                    <button
                      key={h.id}
                      onClick={() => chooseHair(h)}
                      className={`border rounded-xl p-2 ${
                        selectedHair?.id === h.id ? "ring-2 ring-black" : ""
                      }`}
                    >
                      <img src={h.src} alt={h.name} className="w-16 h-12 object-contain" />
                      <div className="text-xs mt-1">{h.name}</div>
                    </button>
                  ))}
                  <button className="px-3 py-2 border rounded-xl" onClick={() => setSelectedHair(null)}>
                    None
                  </button>
                </div>
              </div>

              {/* Filters */}
              <div>
                <label className="block font-medium mb-1">Makeup / Image Filters</label>
                <Slider label="Brightness" value={filters.brightness} min={50} max={150} onChange={(v) => setFilters((f) => ({ ...f, brightness: v }))} />
                <Slider label="Contrast" value={filters.contrast} min={50} max={150} onChange={(v) => setFilters((f) => ({ ...f, contrast: v }))} />
                <Slider label="Saturation" value={filters.saturation} min={50} max={200} onChange={(v) => setFilters((f) => ({ ...f, saturation: v }))} />
                <Slider label="Blur" value={filters.blur} min={0} max={6} step={0.5} onChange={(v) => setFilters((f) => ({ ...f, blur: v }))} />

                <div className="mt-3 space-y-2">
                  <div className="flex items-center gap-2">
                    <input
                      id="blushL"
                      type="checkbox"
                      checked={filters.blushLeft.visible}
                      onChange={(e) => setFilters((f) => ({ ...f, blushLeft: { ...f.blushLeft, visible: e.target.checked } }))}
                    />
                    <label htmlFor="blushL">Blush Left</label>
                    <input
                      type="range"
                      min={10}
                      max={40}
                      value={filters.blushLeft.r}
                      onChange={(e) => setFilters((f) => ({ ...f, blushLeft: { ...f.blushLeft, r: +e.target.value } }))}
                    />
                    <input
                      type="range"
                      min={0.05}
                      max={0.6}
                      step={0.05}
                      value={filters.blushLeft.opacity}
                      onChange={(e) => setFilters((f) => ({ ...f, blushLeft: { ...f.blushLeft, opacity: +e.target.value } }))}
                    />
                  </div>
                  <div className="flex items-center gap-2">
                    <input
                      id="blushR"
                      type="checkbox"
                      checked={filters.blushRight.visible}
                      onChange={(e) => setFilters((f) => ({ ...f, blushRight: { ...f.blushRight, visible: e.target.checked } }))}
                    />
                    <label htmlFor="blushR">Blush Right</label>
                    <input
                      type="range"
                      min={10}
                      max={40}
                      value={filters.blushRight.r}
                      onChange={(e) => setFilters((f) => ({ ...f, blushRight: { ...f.blushRight, r: +e.target.value } }))}
                    />
                    <input
                      type="range"
                      min={0.05}
                      max={0.6}
                      step={0.05}
                      value={filters.blushRight.opacity}
                      onChange={(e) => setFilters((f) => ({ ...f, blushRight: { ...f.blushRight, opacity: +e.target.value } }))}
                    />
                  </div>
                </div>
              </div>

              {/* Suggest & Save */}
              <div className="flex gap-2">
                <button className="px-3 py-2 rounded-xl bg-gray-900 text-white" onClick={suggestOutfit}>
                  Suggest Outfit
                </button>
                <button className="px-3 py-2 rounded-xl border" onClick={saveLook}>
                  Save Look
                </button>
              </div>
            </div>

            {/* Center: Stage */}
            <div className="p-4 bg-white rounded-2xl shadow">
              <h2 className="font-semibold mb-2">Preview & Adjust</h2>
              <Stage
                baseImg={baseImg}
                overlays={overlays}
                filters={filters}
                selected={{ selectedTop, selectedBottom, selectedDress, selectedHair }}
                stageRef={stageRef}
                onDragStart={startDrag}
              />
              <TransformControls overlays={overlays} setOverlays={setOverlays} />
            </div>

            {/* Right: Wardrobe quick pick */}
            <div className="p-4 bg-white rounded-2xl shadow space-y-4">
              <h3 className="font-semibold">Quick Pick</h3>
              <QuickPick label="Tops" items={itemsByCat("top")} onPick={(i) => setSelectedTop(i)} />
              <QuickPick label="Bottoms" items={itemsByCat("bottom")} onPick={(i) => setSelectedBottom(i)} />
              <QuickPick label="Dresses" items={itemsByCat("dress")} onPick={(i) => setSelectedDress(i)} />
            </div>
          </section>
        )}

        {tab === "saved" && (
          <section className="p-4 bg-white rounded-2xl shadow">
            <h2 className="font-semibold text-lg mb-3">Saved Looks</h2>
            {savedLooks.length === 0 && <div className="text-sm text-gray-500">No looks saved yet.</div>}
            <div className="grid grid-cols-2 md:grid-cols-4 gap-4">
              {savedLooks.map((s) => (
                <div key={s.id} className="border rounded-xl overflow-hidden">
                  <img src={s.dataURL} alt="saved look" className="w-full h-48 object-cover" />
                </div>
              ))}
            </div>
          </section>
        )}
      </main>

      <footer className="text-xs text-gray-500 p-6 max-w-6xl mx-auto">
        Built as a college project prototype. Future scope: MediaPipe FaceMesh/Pose for auto-fit, background removal API, and AR live try-on using WebAR/ARCore.
      </footer>
    </div>
  );
}

// ------------------------------
// Stage Component – layers & filters preview
// ------------------------------
function Stage({ baseImg, overlays, filters, selected, stageRef, onDragStart, readonly }) {
  const { selectedTop, selectedBottom, selectedDress, selectedHair } = selected;

  // CSS filters (for live preview only; snapshot applies via canvas processing too)
  const cssFilter = `brightness(${filters.brightness}%) contrast(${filters.contrast}%) saturate(${filters.saturation}%) blur(${filters.blur}px)`;

  return (
    <div
      ref={stageRef}
      className="relative w-full max-w-xl mx-auto aspect-[4/3] bg-gray-100 rounded-xl overflow-hidden border"
    >
      {/* Base image */}
      {baseImg ? (
        <img src={baseImg} alt="selfie" className="absolute inset-0 w-full h-full object-cover" style={{ filter: cssFilter }} />
      ) : (
        <div className="absolute inset-0 flex items-center justify-center text-gray-400 text-sm">
          Upload a selfie to start
        </div>
      )}

      {/* Dress layer (behind tops/bottoms) */}
      {selectedDress && (
        <DraggableImage src={selectedDress.src} overlay={overlays.dress} onDragStart={onDragStart?.bind(null, "dress")} readonly={readonly} />)
      }

      {/* Top & Bottom */}
      {selectedTop && (
        <DraggableImage src={selectedTop.src} overlay={overlays.top} onDragStart={onDragStart?.bind(null, "top")} readonly={readonly} />
      )}
      {selectedBottom && (
        <DraggableImage src={selectedBottom.src} overlay={overlays.bottom} onDragStart={onDragStart?.bind(null, "bottom")} readonly={readonly} />
      )}

      {/* Hair on top */}
      {selectedHair && (
        <DraggableImage src={selectedHair.src} overlay={overlays.hair} onDragStart={onDragStart?.bind(null, "hair")} readonly={readonly} />
      )}

      {/* Blush spots */}
      {filters.blushLeft.visible && (
        <BlushDot x={filters.blushLeft.x} y={filters.blushLeft.y} r={filters.blushLeft.r} opacity={filters.blushLeft.opacity} />
      )}
      {filters.blushRight.visible && (
        <BlushDot x={filters.blushRight.x} y={filters.blushRight.y} r={filters.blushRight.r} opacity={filters.blushRight.opacity} />
      )}
    </div>
  );
}

function DraggableImage({ src, overlay, onDragStart, readonly }) {
  const style = {
    position: "absolute",
    left: overlay.x,
    top: overlay.y,
    transform: `scale(${overlay.scale}) rotate(${overlay.rot}deg)`,
    transformOrigin: "top left",
    width: "200px", // base width (scaled by transform)
    cursor: readonly ? "default" : "grab",
  };
  return (
    <img
      src={src}
      alt="overlay"
      style={style}
      draggable={false}
      onMouseDown={readonly ? undefined : onDragStart}
    />
  );
}

function BlushDot({ x, y, r, opacity }) {
  return (
    <div
      className="absolute rounded-full"
      style={{ left: x - r, top: y - r, width: r * 2, height: r * 2, background: "#ff3366", opacity }}
    />
  );
}

// ------------------------------
// Controls
// ------------------------------
function TransformControls({ overlays, setOverlays }) {
  return (
    <div className="mt-4 grid md:grid-cols-2 gap-4">
      {(["top", "bottom", "dress", "hair"]).map((key) => (
        <div key={key} className="p-3 border rounded-xl">
          <div className="font-medium capitalize mb-2">{key} Transform</div>
          <div className="grid grid-cols-2 gap-2 items-center text-sm">
            <span>Scale</span>
            <input
              type="range"
              min={0.2}
              max={2}
              step={0.05}
              value={overlays[key].scale}
              onChange={(e) => setOverlays((ov) => ({ ...ov, [key]: { ...ov[key], scale: +e.target.value } }))}
            />
            <span>Rotate</span>
            <input
              type="range"
              min={-45}
              max={45}
              step={1}
              value={overlays[key].rot}
              onChange={(e) => setOverlays((ov) => ({ ...ov, [key]: { ...ov[key], rot: +e.target.value } }))}
            />
          </div>
        </div>
      ))}
    </div>
  );
}

function Picker({ label, items, selected, onSelect, onClear }) {
  return (
    <div>
      <label className="block font-medium mb-1">{label}</label>
      <div className="flex gap-2 flex-wrap">
        {items.map((i) => (
          <button
            key={i.id}
            onClick={() => onSelect(i.id)}
            className={`border rounded-xl p-2 ${selected === i.id ? "ring-2 ring-black" : ""}`}
          >
            <img src={i.src} alt={i.name} className="w-16 h-12 object-contain" />
            <div className="text-xs mt-1 max-w-[64px] truncate" title={i.name}>{i.name}</div>
          </button>
        ))}
        <button className="px-3 py-2 border rounded-xl" onClick={onClear}>None</button>
      </div>
    </div>
  );
}

function AddClothCard({ title, category, inputId, onFile }) {
  return (
    <div className="p-4 bg-white rounded-2xl shadow">
      <h3 className="font-semibold mb-2">{title}</h3>
      <div className="space-y-2">
        <input id={inputId} placeholder={`${category} name`} className="w-full border rounded-lg px-3 py-2" />
        <input type="file" accept="image/*" onChange={onFile} />
        <p className="text-xs text-gray-500">Tip: Transparent PNGs work best for overlays. You can also upload JPGs.</p>
      </div>
    </div>
  );
}

function WardrobeGrid({ items, onRemove }) {
  if (items.length === 0) return <div className="text-sm text-gray-500">No items added yet.</div>;
  return (
    <div className="grid grid-cols-2 md:grid-cols-4 lg:grid-cols-6 gap-3">
      {items.map((i) => (
        <div key={i.id} className="border rounded-xl p-2 flex flex-col">
          <img src={i.src} alt={i.name} className="w-full h-28 object-contain" />
          <div className="text-xs mt-1 font-medium">{i.name}</div>
          <div className="text-[10px] text-gray-500">{i.category}</div>
          <button className="mt-2 text-xs px-2 py-1 border rounded-lg self-start" onClick={() => onRemove(i.id)}>
            Remove
          </button>
        </div>
      ))}
    </div>
  );
}

function QuickPick({ label, items, onPick }) {
  return (
    <div>
      <div className="font-medium mb-1">{label}</div>
      <div className="flex gap-2 overflow-auto">
        {items.map((i) => (
          <button key={i.id} onClick={() => onPick(i)} className="border rounded-xl p-2 min-w-[90px]">
            <img src={i.src} alt={i.name} className="w-16 h-12 object-contain mx-auto" />
            <div className="text-[10px] mt-1 text-center truncate" title={i.name}>{i.name}</div>
          </button>
        ))}
        {items.length === 0 && <div className="text-xs text-gray-500">Empty</div>}
      </div>
    </div>
  );
}

function Slider({ label, value, min = 0, max = 100, step = 1, onChange }) {
  return (
    <div className="grid grid-cols-[120px_1fr_60px] items-center gap-2 text-sm my-1">
      <span>{label}</span>
      <input type="range" min={min} max={max} step={step} value={value} onChange={(e) => onChange(+e.target.value)} />
      <span className="text-right tabular-nums">{value}</span>
    </div>
  );
}

// ------------------------------
// Canvas helpers for saving snapshot with filters
// ------------------------------
function loadImage(src) {
  return new Promise((res, rej) => {
    const img = new Image();
    img.crossOrigin = "anonymous";
    img.onload = () => res(img);
    img.onerror = rej;
    img.src = src;
  });
}

// Apply brightness, contrast, saturation roughly via pixel ops
function applyCanvasFilters(ctx, w, h, filters) {
  const imgData = ctx.getImageData(0, 0, w, h);
  const d = imgData.data;

  const b = filters.brightness / 100; // 0.5..1.5
  const c = filters.contrast / 100; // 0.5..1.5
  const s = filters.saturation / 100; // 0.5..2

  for (let i = 0; i < d.length; i += 4) {
    let r = d[i], g = d[i + 1], bl = d[i + 2];

    // Brightness
    r = r * b; g = g * b; bl = bl * b;

    // Contrast (pivot 128)
    r = (r - 128) * c + 128;
    g = (g - 128) * c + 128;
    bl = (bl - 128) * c + 128;

    // Saturation (HSL approx) – compute luminance and interpolate
    const avg = (r + g + bl) / 3;
    r = avg + (r - avg) * s;
    g = avg + (g - avg) * s;
    bl = avg + (bl - avg) * s;

    d[i] = clamp(r, 0, 255);
    d[i + 1] = clamp(g, 0, 255);
    d[i + 2] = clamp(bl, 0, 255);
  }

  ctx.putImageData(imgData, 0, 0);

  // Simple blur using canvas filter then redraw onto temp canvas (optional, mild)
  if (filters.blur > 0) {
    const tmp = document.createElement("canvas");
    tmp.width = w; tmp.height = h;
    const tctx = tmp.getContext("2d");
    tctx.filter = `blur(${filters.blur}px)`;
    tctx.drawImage(ctx.canvas, 0, 0);
    ctx.clearRect(0, 0, w, h);
    ctx.drawImage(tmp, 0, 0);
  }
}

function clamp(v, min, max) { return Math.max(min, Math.min(max, v)); }