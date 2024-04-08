#include "CambR.hpp"

CambR::CambR(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z, int min_grid_resolution, int max_grid_resolution) : sam_unit_(min_x, max_x, min_y, max_y, min_z, max_z), enc_unit_(min_grid_resolution, max_grid_resolution) {
    for (unsigned int i = 0; i < 128; ++i) {
        MlpUnit tem_mlpunit(i);
        mlp_units_.push_back(tem_mlpunit);
    }
}

void CambR::Cycle() {

}
/*
######################################
def Cycle():
	SAM.Cycle()
	ENC.Cycle()
	for MLP in MLPs:
		MLP.Cycle()

	# Check SAM output (0 or 8 batches of samples) and change appropriate element of 'states' from SAM_IN_PROG -> BEF_ENC.
	if (SAM has output):
		ridx = output.ridx
		states[ridx % 128] = BEF_ENC
		samples[ridx] = output

	# Check ENC outputs (0 or more batches of features) and change appropriate element of 'states' from ENC_IN_PROG -> READY.
	# And of course, store the feature batch in the appropriate element of 'features'.
	if (ENC has output):
		ridx = output.ridx
		states[ridx % 128] = READY
		features[ridx % 128] = output

	# Check idle MLP and feed them features from 'features'.
	# And change state of 'states' too.
	for i in range(128):
		MLP = MLPs[i]
		feature = features[i]
		state = states[i]
		if MLP.is_idle() && state == READY:
			if MLP has output:
				out_pixels.append(output)
				if feature.bidx == 0: # Non-early termination.
					MLP.in_features = feature
					features[i] = (empty feature batch with ridx and bidx(=1) of next batch)
					states[i] = BEF_ENC
				else: # Early termination.
					features[i] = (empty feature batch with ridx of next ray and bidx of 0)
					states[i] = BEF_SAM
			else:
				MLP.in_features = feature
				features[i] = (empty feature batch with ridx and bidx of next batch)
				if features[i].bidx == 0: # Next batch will come from new ray.
					states[i] = BEF_SAM # New ray, new sampling!
				else:
					states[i] = BEF_ENC # This ray is already sampled.

	for i in range(128):
		if states[i] == BEF_SAM:
			SAM.in_ray = rays[features[i].ridx]
			states[i] = SAM_IN_PROG
			break

	for i in range(128):
		if states[i] == BEF_ENC:
			ENC.in_samples = samples[features[i].ridx][features[i].bidx]
			states[i] = ENC_IN_PROG
			break

##################################################
*/