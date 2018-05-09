"""
Experimenting with a flexible PSF model.
"""

type PSF
    """
    A PSF class.
    """

    # Fourier coefficients for the angular structure
    sin_coeffs::Array{Float64, 1}
    cos_coeffs::Array{Float64, 1}
end

function PSF()
    """
    Default constructor
    """
    return PSF(randn(10), randn(10))
end

